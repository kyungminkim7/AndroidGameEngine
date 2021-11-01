#include <network/TcpSubscriber.h>

#include <chrono>
#include <system_error>

#include <asio/read.hpp>
#include <asio/write.hpp>

#include <network/Utils.h>
#include <network/msgs/Header_generated.h>
#include <network/msgs/MsgCtrl_generated.h>

namespace {

constexpr auto SOCKET_RECONNECT_WAIT_DURATION = std::chrono::milliseconds(30);

} // namespace

namespace ntwk {

using namespace asio::ip;

std::shared_ptr<TcpSubscriber> TcpSubscriber::create(asio::io_context &mainContext,
                                                     asio::io_context &subscriberContext,
                                                     const std::string &host,
                                                     unsigned short port) {
    std::shared_ptr<TcpSubscriber> subscriber(new TcpSubscriber(mainContext, subscriberContext,
                                                                host, port));
    connect(subscriber);
    return subscriber;
}

TcpSubscriber::TcpSubscriber(asio::io_context &mainContext, asio::io_context &subscriberContext,
                             const std::string &host, unsigned short port) :
    mainContext(mainContext), subscriberContext(subscriberContext),
    socket(subscriberContext), endpoint(make_address(host), port) {}

void TcpSubscriber::subscribe(MsgTypeId msgTypeId, MsgHandler msgHandler) {
    this->msgHandlers[toUnderlyingType(msgTypeId)] = std::move(msgHandler);
}

void TcpSubscriber::connect(std::shared_ptr<TcpSubscriber> subscriber) {
    auto pSubscriber = subscriber.get();

    pSubscriber->socket.async_connect(pSubscriber->endpoint,
                                      [pSubscriber, subscriber=std::move(subscriber)](const auto &error) mutable {
        if (error) {
            subscriber->socket.close();

            subscriber->socketReconnectTimer = std::make_unique<asio::steady_timer>(subscriber->subscriberContext,
                                                                                    SOCKET_RECONNECT_WAIT_DURATION);
            pSubscriber->socketReconnectTimer->async_wait([pSubscriber, subscriber=std::move(subscriber)]
                                                          (const auto &error) mutable {
                connect(std::move(subscriber));
            });
        } else {
            receiveMsg(std::move(subscriber));
        }
    });
}

void TcpSubscriber::receiveMsg(std::shared_ptr<TcpSubscriber> &&subscriber) {
    // Wait for msg header
    auto msgHeader = std::make_unique<msgs::Header>();
    auto pMsgHeader = msgHeader.get();
    auto pSubscriber = subscriber.get();
    asio::async_read(pSubscriber->socket, asio::buffer(pMsgHeader, sizeof(msgs::Header)),
                     [subscriber=std::move(subscriber), msgHeader=std::move(msgHeader), pMsgHeader]
                     (const auto &error, auto bytesReceived) mutable {
        try {
            if (error) {
                throw std::system_error(std::make_error_code(std::io_errc::stream));
            }

            // Receive the rest of the msg header
            if (bytesReceived < sizeof(msgs::Header)) {
                asio::read(subscriber->socket, asio::buffer(pMsgHeader + bytesReceived,
                                                            sizeof(msgs::Header) - bytesReceived));
            }

            // Receive msg
            auto msg = std::make_unique<uint8_t[]>(msgHeader->msg_size());
            asio::read(subscriber->socket, asio::buffer(msg.get(), msgHeader->msg_size()));

            // Enqueue msg for handling (only process latest msg)
            const auto msgTypeId = msgHeader->msg_type_id();
            if (!subscriber->msgBuffers[msgTypeId]) {
                asio::post(subscriber->mainContext, [subscriber, msgTypeId]() mutable {
                    postMsgHandlingTask(std::move(subscriber), msgTypeId);
                });
            }
            subscriber->msgBuffers[msgTypeId] = std::move(msg);

            // Acknowledge msg reception
            auto ack = msgs::MsgCtrl::ACK;
            asio::write(subscriber->socket, asio::buffer(&ack, sizeof(msgs::MsgCtrl)));

        } catch (...) {
            subscriber->socket.close();
            connect(std::move(subscriber));
            return;
        }

        receiveMsg(std::move(subscriber));
    });
}

void TcpSubscriber::postMsgHandlingTask(std::shared_ptr<TcpSubscriber> &&subscriber,
                                        MsgTypeIdUnderlyingType msgTypeId) {
    auto pSubscriber = subscriber.get();
    asio::post(pSubscriber->subscriberContext,
               [pSubscriber, subscriber=std::move(subscriber), msgTypeId]() mutable {
        asio::post(pSubscriber->mainContext,
                   [subscriber=std::move(subscriber), msgTypeId,
                   msg=std::move(pSubscriber->msgBuffers[msgTypeId])]() mutable {
            auto handler = subscriber->msgHandlers.find(msgTypeId);
            if (handler != subscriber->msgHandlers.end()) {
                handler->second(std::move(msg));
            }
        });
    });
}

} // namespace ntwk
