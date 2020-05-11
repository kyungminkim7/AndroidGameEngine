#pragma once

#include <list>
#include <memory>
#include <queue>

#include <asio/ip/tcp.hpp>
#include <asio/io_context.hpp>
#include <std_msgs/Header_generated.h>

namespace ntwk {

class TcpPublisher : public std::enable_shared_from_this<TcpPublisher> {
public:
    static std::shared_ptr<TcpPublisher> create(asio::io_context &ioContext,
                                                unsigned short port,
                                                unsigned int msgQueueSize);

    void publish(std::shared_ptr<flatbuffers::DetachedBuffer> msg);

    void update();

private:
    TcpPublisher(asio::io_context &ioContext, unsigned short port, unsigned int msgQueueSize);
    void listenForConnections();
    void removeSocket(asio::ip::tcp::socket *socket);

    static void sendMsgHeader(std::shared_ptr<ntwk::TcpPublisher> publisher,
                              asio::ip::tcp::socket *socket,
                              std::shared_ptr<std_msgs::Header> msgHeader,
                              std::shared_ptr<flatbuffers::DetachedBuffer> msg,
                              unsigned int totalMsgHeaderBytesTransferred);

    static void sendMsg(std::shared_ptr<ntwk::TcpPublisher> publisher,
                        asio::ip::tcp::socket *socket,
                        std::shared_ptr<flatbuffers::DetachedBuffer> msg,
                        unsigned int totalMsgBytesTransferred);

    asio::io_context &ioContext;
    asio::ip::tcp::acceptor socketAcceptor;

    std::list<std::unique_ptr<asio::ip::tcp::socket>> connectedSockets;

    std::queue<std::shared_ptr<flatbuffers::DetachedBuffer>> msgQueue;
    unsigned int msgQueueSize;
    std::weak_ptr<flatbuffers::DetachedBuffer> msgBeingSent;
};

} // namespace ntwk
