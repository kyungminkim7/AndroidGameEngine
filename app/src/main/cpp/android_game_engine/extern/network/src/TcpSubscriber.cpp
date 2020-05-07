#include <network/TcpSubscriber.h>

#include <asio/read.hpp>

#include <std_msgs/Header_generated.h>

namespace ntwk {

using namespace asio::ip;

std::shared_ptr<TcpSubscriber> TcpSubscriber::create(asio::io_context &ioContext,
                                                     const std::string &host, unsigned short port,
                                                     MessageReceivedHandler msgReceivedHandler) {
    std::shared_ptr<TcpSubscriber> subscriber(new TcpSubscriber(ioContext,
                                                                host, port,
                                                                std::move(msgReceivedHandler)));
    subscriber->connect(subscriber);
    return subscriber;
}

TcpSubscriber::TcpSubscriber(asio::io_context &ioContext,
                             const std::string &host, unsigned short port,
                             MessageReceivedHandler msgReceivedHandler) :
    socket(ioContext),
    endpoint(make_address(host), port),
    msgReceivedHandler(std::move(msgReceivedHandler)) { }

void TcpSubscriber::connect(std::shared_ptr<TcpSubscriber> subscriber) {
    this->socket.async_connect(this->endpoint, [subscriber=std::move(subscriber)](const auto &error) mutable {
        auto pSubscriber = subscriber.get();

        if (error) {
            // Try again to connect
            pSubscriber->connect(std::move(subscriber));
        } else {
            // Start receiving messages
            pSubscriber->receiveMsgHeader(std::move(subscriber));
        }
    });
}

void TcpSubscriber::receiveMsgHeader(std::shared_ptr<TcpSubscriber> subscriber) {
    auto msgHeader = std::make_unique<std_msgs::Header>();
    auto pMsgHeader = msgHeader.get();
    asio::async_read(this->socket, asio::buffer(pMsgHeader, sizeof(std_msgs::Header)),
                     [subscriber=std::move(subscriber), msgHeader=std::move(msgHeader)](const auto &error, auto bytesTransferred) mutable {
        auto pSubscriber = subscriber.get();

        if (error) {
            subscriber->socket.close();
            pSubscriber->connect(std::move(subscriber));
        } else {
            pSubscriber->receiveMsg(std::move(subscriber), msgHeader->msgSize());
        }
    });
}

void TcpSubscriber::receiveMsg(std::shared_ptr<TcpSubscriber> subscriber, std::size_t msgSize_bytes) {
    auto msg = std::make_unique<uint8_t[]>(msgSize_bytes);
    auto pMsg = msg.get();
    asio::async_read(this->socket, asio::buffer(pMsg, msgSize_bytes),
                     [subscriber=std::move(subscriber), msg=std::move(msg)](const auto &error, auto bytesTransferred) mutable {
        auto pSubscriber = subscriber.get();

        if (error) {
            subscriber->socket.close();
            pSubscriber->connect(std::move(subscriber));
        } else {
            subscriber->msgReceivedHandler(std::move(msg));
            pSubscriber->receiveMsgHeader(std::move(subscriber));
        }
    });
}

} // namespace ntwk
