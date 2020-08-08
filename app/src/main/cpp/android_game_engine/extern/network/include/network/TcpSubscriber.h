#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>

#include <asio/ip/tcp.hpp>
#include <asio/steady_timer.hpp>
#include <std_msgs/Header_generated.h>
#include <std_msgs/MessageControl_generated.h>

namespace ntwk {

struct Image;

template<typename T, typename DecompressionPolicy>
class TcpSubscriber {
public:
    using MsgReceivedHandler = std::function<void(std::unique_ptr<T>)>;

    static std::shared_ptr<TcpSubscriber> create(asio::io_context &mainContext,
                                                 asio::io_context &subscriberContext,
                                                 const std::string &host, unsigned short port,
                                                 MsgReceivedHandler msgReceivedHandler);

private:
    TcpSubscriber(asio::io_context &mainContext,
                  asio::io_context &subscriberContext,
                  const std::string &host, unsigned short port,
                  MsgReceivedHandler msgReceivedHandler);

    static void connect(std::shared_ptr<TcpSubscriber> subscriber);

    static void receiveMsgHeader(std::shared_ptr<TcpSubscriber> subscriber,
                                 std::unique_ptr<std_msgs::Header> msgHeader,
                                 unsigned int totalMsgHeaderBytesReceived);

    static void receiveMsg(std::shared_ptr<TcpSubscriber> subscriber,
                           std::unique_ptr<uint8_t[]> msg,
                           unsigned int msgSize_bytes, unsigned int totalMsgBytesReceived);

    static void processMsg(std::shared_ptr<TcpSubscriber> subscriber,
                           std::unique_ptr<uint8_t[]> msgBuffer);
    static void postMsgHandlingTask(std::shared_ptr<TcpSubscriber> subscriber);
    static void postImgMsgHandlingTask(std::shared_ptr<TcpSubscriber> subscriber);

    static void sendMsgControl(std::shared_ptr<TcpSubscriber> subscriber,
                               std::unique_ptr<std_msgs::MessageControl> msgCtrl,
                               unsigned int totalMsgCtrlBytesTransferred);

private:
    asio::io_context &mainContext;
    asio::io_context &subscriberContext;

    asio::ip::tcp::socket socket;
    std::mutex socketMutex;

    asio::ip::tcp::endpoint endpoint;

    std::unique_ptr<asio::steady_timer> socketReconnectTimer;

    MsgReceivedHandler msgReceivedHandler;

    // Queues for double buffering msgs
    std::queue<std::unique_ptr<T>> msgQueue;
    std::mutex msgQueueMutex;
    static const unsigned int MSG_QUEUE_SIZE = 2u;
};

} // namespace ntwk

#include "TcpSubscriber_impl.h"
