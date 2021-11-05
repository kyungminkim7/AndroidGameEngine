#include <network/TcpPublisher.h>

#include <algorithm>
#include <system_error>
#include <unordered_map>

#include <asio/read.hpp>
#include <asio/write.hpp>

#include <network/Utils.h>
#include <network/msgs/Header_generated.h>
#include <network/msgs/MsgCtrl_generated.h>

namespace ntwk {

struct Msg;

using namespace asio::ip;
using MsgMap = std::unordered_map<MsgTypeId, Msg>;

struct Msg {
    std::shared_ptr<msgs::Header> header;
    std::shared_ptr<flatbuffers::DetachedBuffer> buffer;
};

struct TcpPublisher::Socket {
    tcp::socket socket;
    MsgMap msgs;

    explicit Socket(asio::io_context &context) : socket(context) {}
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
            auto &msgBuffer = socket->msgs[msgTypeId];

            // Schedule msg to be sent if available
            if (!msgBuffer.buffer)  {
                asio::post(publisher->publisherContext,
                           [publisher, socket, msgTypeId]() mutable {
                    TcpPublisher::sendMsg(std::move(publisher), std::move(socket),
                                          msgTypeId);
                });
            }

            // Enqueue msg to send
            msgBuffer.header = header;
            msgBuffer.buffer = msg;
        }
    });
}

void TcpPublisher::sendMsg(PublisherPtr &&publisher, SocketPtr &&socket,
                           MsgTypeId msgTypeId) {
    auto &msg = socket->msgs[msgTypeId];

    try {
        // Send msg header and data
        asio::write(socket->socket, asio::buffer(msg.header.get(), sizeof(msgs::Header)));
        asio::write(socket->socket, asio::buffer(msg.buffer->data(), msg.buffer->size()));

        // Wait for ack
        msgs::MsgCtrl msgCtrl;
        asio::read(socket->socket, asio::buffer(&msgCtrl, sizeof(msgs::MsgCtrl)));
        if (msgCtrl != msgs::MsgCtrl::ACK) {
            throw std::system_error(std::make_error_code(std::io_errc::stream));
        }

    } catch (...) {
        auto iter = std::find_if(publisher->connectedSockets.cbegin(), publisher->connectedSockets.cend(),
                                 [socket=socket.get()](const auto &s){ return s.get() == socket; });
        if (iter != publisher->connectedSockets.cend()) {
            publisher->connectedSockets.erase(iter);
        }
    }

    msg.header.reset();
    msg.buffer.reset();
}

} // namespace ntwk
