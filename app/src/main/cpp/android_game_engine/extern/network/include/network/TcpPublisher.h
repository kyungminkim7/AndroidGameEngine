#pragma once

#include <atomic>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <utility>
#include <vector>

#include <asio/ip/tcp.hpp>
#include <asio/io_context.hpp>
#include <std_msgs/Header_generated.h>
#include <std_msgs/MessageControl_generated.h>

#include "Compression.h"

namespace ntwk {

class TcpPublisher : public std::enable_shared_from_this<TcpPublisher> {
public:
    static std::shared_ptr<TcpPublisher> create(asio::io_context &ioContext,
                                                unsigned short port,
                                                unsigned int msgQueueSize,
                                                Compression compression);

    void publish(std::shared_ptr<flatbuffers::DetachedBuffer> msg);
    void publishImage(unsigned int width, unsigned int height, uint8_t channels,
                      const uint8_t data[]);

    void update();

private:
    // Denotes a socket connection and a flag indicating whether it's ready to send data
    using Socket = std::pair<std::unique_ptr<asio::ip::tcp::socket>, std::atomic<bool>>;

    TcpPublisher(asio::io_context &ioContext, unsigned short port,
                 unsigned int msgQueueSize, Compression compression);
    void listenForConnections();
    void removeSocket(Socket *socket);

    static void sendMsgHeader(std::shared_ptr<ntwk::TcpPublisher> publisher, Socket *socket,
                              std::shared_ptr<const std_msgs::Header> msgHeader,
                              std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                              unsigned int totalMsgHeaderBytesTransferred);

    static void sendMsg(std::shared_ptr<ntwk::TcpPublisher> publisher, Socket *socket,
                        std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                        unsigned int totalMsgBytesTransferred);

    static void receiveMsgControl(std::shared_ptr<TcpPublisher> publisher, Socket *socket,
                                  std::unique_ptr<std_msgs::MessageControl> msgCtrl,
                                  unsigned int totalMsgCtrlBytesReceived);

    asio::io_context &ioContext;
    asio::ip::tcp::acceptor socketAcceptor;

    std::list<Socket> connectedSockets;
    std::mutex socketsMutex;

    std::queue<std::shared_ptr<flatbuffers::DetachedBuffer>> msgQueue;
    unsigned int msgQueueSize;
    std::mutex msgQueueMutex;
    std::weak_ptr<flatbuffers::DetachedBuffer> msgBeingSent;

    Compression compression;
};

} // namespace ntwk
