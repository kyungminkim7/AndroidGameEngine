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

#include "Compression.h"

namespace ntwk {

struct Image;

class TcpSubscriber : public std::enable_shared_from_this<TcpSubscriber> {
public:
    using MsgReceivedHandler = std::function<void(std::unique_ptr<uint8_t[]>)>;
    using ImageMsgReceivedHandler = std::function<void(std::unique_ptr<Image>)>;

    static std::shared_ptr<TcpSubscriber> create(asio::io_context &mainContext,
                                                 asio::io_context &subscriberContext,
                                                 const std::string &host, unsigned short port,
                                                 MsgReceivedHandler msgReceivedHandler,
                                                 Compression compression);

    static std::shared_ptr<TcpSubscriber> create(asio::io_context &mainContext,
                                                 asio::io_context &subscriberContext,
                                                 const std::string &host, unsigned short port,
                                                 ImageMsgReceivedHandler imgMsgReceivedHandler,
                                                 Compression compression);

private:
    TcpSubscriber(asio::io_context &mainContext,
                  asio::io_context &subscriberContext,
                  const std::string &host, unsigned short port,
                  MsgReceivedHandler msgReceivedHandler,
                  Compression compression);

    TcpSubscriber(asio::io_context &mainContext,
                  asio::io_context &subscriberContext,
                  const std::string &host, unsigned short port,
                  ImageMsgReceivedHandler imgMsgReceivedHandler,
                  Compression compression);

    static void connect(std::shared_ptr<TcpSubscriber> subscriber);

    static void receiveMsgHeader(std::shared_ptr<TcpSubscriber> subscriber,
                                 std::unique_ptr<std_msgs::Header> msgHeader,
                                 unsigned int totalMsgHeaderBytesReceived);

    static void receiveMsg(std::shared_ptr<TcpSubscriber> subscriber,
                           std::unique_ptr<uint8_t[]> msg,
                           unsigned int msgSize_bytes, unsigned int totalMsgBytesReceived);

    static void processMsg(std::shared_ptr<TcpSubscriber> subscriber, std::unique_ptr<uint8_t[]> msg);
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
    ImageMsgReceivedHandler imgMsgReceivedHandler;

    // Queues for double buffering msgs
    std::queue<std::unique_ptr<uint8_t[]>> msgQueue;
    std::queue<std::unique_ptr<Image>> imgMsgQueue;
    std::mutex msgQueueMutex;
    static const unsigned int MSG_QUEUE_SIZE = 2u;

    Compression compression;
};

} // namespace ntwk
