#pragma once

#include <list>
#include <memory>
#include <mutex>
#include <queue>
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
    TcpPublisher(asio::io_context &ioContext, unsigned short port,
                 unsigned int msgQueueSize, Compression compression);

    void listenForConnections();
    void removeSocket(asio::ip::tcp::socket *socket);

    static void sendMsgHeader(std::shared_ptr<ntwk::TcpPublisher> publisher,
                              asio::ip::tcp::socket *socket,
                              std::shared_ptr<const std_msgs::Header> msgHeader,
                              std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                              unsigned int totalMsgHeaderBytesTransferred);

    static void sendMsg(std::shared_ptr<ntwk::TcpPublisher> publisher,
                        asio::ip::tcp::socket *socket,
                        std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                        unsigned int totalMsgBytesTransferred);

    static void receiveMsgControl(std::shared_ptr<TcpPublisher> publisher,
                                  asio::ip::tcp::socket *socket,
                                  std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                                  std::unique_ptr<std_msgs::MessageControl> msgCtrl,
                                  unsigned int totalMsgCtrlBytesReceived);

    asio::io_context &ioContext;
    asio::ip::tcp::acceptor socketAcceptor;

    std::list<std::unique_ptr<asio::ip::tcp::socket>> connectedSockets;
    std::mutex socketsMutex;

    std::queue<std::shared_ptr<flatbuffers::DetachedBuffer>> msgQueue;
    unsigned int msgQueueSize;
    std::mutex msgQueueMutex;
    std::weak_ptr<flatbuffers::DetachedBuffer> msgBeingSent;

    Compression compression;
};

} // namespace ntwk
