#include <network/TcpPublisher.h>

#include <asio/write.hpp>

#include <std_msgs/Header_generated.h>

namespace ntwk {

using namespace asio::ip;

std::shared_ptr<TcpPublisher> TcpPublisher::create(asio::io_context &ioContext, unsigned short port) {
    std::shared_ptr<TcpPublisher> publisher(new TcpPublisher(ioContext, port));
    publisher->listenForConnections();
    return publisher;
}

TcpPublisher::TcpPublisher(asio::io_context &ioContext, unsigned short port) :
    ioContext(ioContext),
    socketAcceptor(ioContext, tcp::endpoint(tcp::v4(), port)) { }

void TcpPublisher::listenForConnections() {
    auto socket = std::make_unique<tcp::socket>(this->ioContext);
    auto pSocket = socket.get();

    // Save connected sockets for later publishing and listen for more connections
    this->socketAcceptor.async_accept(*pSocket,
                                      [publisher=shared_from_this(), socket=std::move(socket)](const auto &error) mutable {
        if (error) {
            throw asio::system_error(error);
        }

        {
            std::lock_guard<std::mutex> guard(publisher->socketsMutex);
            publisher->connectedSockets.emplace_back(std::move(socket));
        }

        publisher->listenForConnections();
    });
}

void TcpPublisher::removeSocket(tcp::socket *socket) {
    std::lock_guard<std::mutex> guard(this->socketsMutex);
    for (auto iter = this->connectedSockets.cbegin(); iter != this->connectedSockets.cend(); ) {
        if (iter->get() == socket) {
            iter = this->connectedSockets.erase(iter);
            return;
        } else {
            ++iter;
        }
    }
}

template<>
void TcpPublisher::publish(std::shared_ptr<flatbuffers::DetachedBuffer> msg) {
    auto msgHeader = std::make_shared<std_msgs::Header>(msg->size());

    {
        std::lock_guard<std::mutex> guard(this->socketsMutex);
        for (auto &socket : this->connectedSockets) {
            // Publish msg header
            asio::async_write(*socket, asio::buffer(msgHeader.get(), sizeof(std_msgs::Header)),
                              [publisher=shared_from_this(), socket=socket.get(), msgHeader, msg](const auto &error, auto bytesTransferred) mutable {
                if (error) {
                    publisher->removeSocket(socket);
                    return;
                }

                // Publish msg
                auto pMsg = msg.get();
                asio::async_write(*socket, asio::buffer(pMsg->data(), pMsg->size()),
                                  [publisher=std::move(publisher), socket, msg=std::move(msg)](const auto &error, auto bytesTransferred){
                    if (error) {
                        publisher->removeSocket(socket);
                        return;
                    }
                });
            });
        }
    }
}

} // namespace ntwk
