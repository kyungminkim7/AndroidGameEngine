#include <network/TcpPublisher.h>

#include <algorithm>
#include <system_error>

#include <asio/read.hpp>
#include <asio/write.hpp>

#include <network/Utils.h>
#include <network/msgs/Header_generated.h>
#include <network/msgs/MsgCtrl_generated.h>

namespace ntwk {

using namespace asio::ip;

struct TcpPublisher::Socket {
    tcp::socket socket;
    bool available;

    explicit Socket(asio::io_context &context) : socket(context), available(true) {}
};

std::shared_ptr<TcpPublisher> TcpPublisher::create(asio::io_context &publisherContext,
                                                   unsigned short port) {
    std::shared_ptr<TcpPublisher> publisher(new TcpPublisher(publisherContext, port));
    publisher->listenForConnections();
    return publisher;
}

TcpPublisher::TcpPublisher(asio::io_context &publisherContext, unsigned short port) :
    publisherContext(publisherContext),
    socketAcceptor(publisherContext, tcp::endpoint(tcp::v4(), port)) { }

void TcpPublisher::listenForConnections() {
    auto socket = std::make_unique<Socket>(this->publisherContext);
    auto pSocket = socket.get();

    // Save connected sockets for later publishing and listen for more connections
    this->socketAcceptor.async_accept(pSocket->socket,
                                      [publisher=this->shared_from_this(),
                                       socket=std::move(socket)](const auto &error) mutable {
        if (!error) {
            publisher->connectedSockets.emplace_back(std::move(socket));
        }
        publisher->listenForConnections();
    });
}

void TcpPublisher::publish(MsgTypeId msgTypeId,
                           std::shared_ptr<flatbuffers::DetachedBuffer> msg) {
    asio::post(this->publisherContext,
               [publisher=this->shared_from_this(), msgTypeId, msg=std::move(msg)]() mutable {
        auto header = std::make_shared<msgs::Header>(toUnderlyingType(msgTypeId), msg->size());

        for (auto &socket : publisher->connectedSockets) {
            if (socket->available) {
                socket->available = false;

                // Send the header, then the msg, and wait for an ACK
                asio::async_write(socket->socket, asio::buffer(header.get(), sizeof(msgs::Header)),
                                  [publisher, socket=socket.get(), header, msg](const auto &error, auto bytesSent){
                    try {
                        if (error) {
                            throw std::system_error(std::make_error_code(std::io_errc::stream));
                        }

                        // Send the rest of the header
                        if (bytesSent < sizeof(msgs::Header)) {
                            asio::write(socket->socket, asio::buffer(header.get() + bytesSent,
                                                              sizeof(msgs::Header) - bytesSent));
                        }

                        // Send msg
                        asio::write(socket->socket, asio::buffer(msg->data(), msg->size()));

                        // Wait for ack
                        msgs::MsgCtrl msgCtrl;
                        asio::read(socket->socket, asio::buffer(&msgCtrl, sizeof(msgs::MsgCtrl)));
                        if (msgCtrl != msgs::MsgCtrl::ACK) {
                            throw std::system_error(std::make_error_code(std::io_errc::stream));
                        }

                        socket->available = true;
                    } catch (...) {
                        auto iter = std::find_if(publisher->connectedSockets.cbegin(), publisher->connectedSockets.cend(),
                                                 [socket](const auto &s){ return s.get() == socket; });
                        publisher->connectedSockets.erase(iter);
                    }
                });
            }
        }
    });
}

} // namespace ntwk
