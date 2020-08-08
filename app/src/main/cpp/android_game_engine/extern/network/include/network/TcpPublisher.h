#pragma once

#include <list>
#include <memory>
#include <vector>

#include <asio/ip/tcp.hpp>
#include <asio/io_context.hpp>
#include <std_msgs/Header_generated.h>
#include <std_msgs/MessageControl_generated.h>

namespace ntwk {

template<typename CompressionPolicy>
class TcpPublisher : public std::enable_shared_from_this<TcpPublisher<CompressionPolicy>> {
public:
    static std::shared_ptr<TcpPublisher> create(asio::io_context &publisherContext,
                                                unsigned short port);

    void publish(std::shared_ptr<flatbuffers::DetachedBuffer> msg);
    void publish(unsigned int width, unsigned int height, uint8_t channels, const uint8_t data[]);

private:
    struct Socket {
        std::unique_ptr<asio::ip::tcp::socket> socket;
        bool readyToWrite;

        explicit Socket(std::unique_ptr<asio::ip::tcp::socket> socket) :
            socket(std::move(socket)), readyToWrite(true){}
    };

    TcpPublisher(asio::io_context &publisherContext, unsigned short port);

    void listenForConnections();
    void removeSocket(Socket *socket);

    static void sendMsgHeader(std::shared_ptr<ntwk::TcpPublisher<CompressionPolicy>> publisher, Socket *socket,
                              std::shared_ptr<const std_msgs::Header> msgHeader,
                              std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                              unsigned int totalMsgHeaderBytesTransferred);

    static void sendMsg(std::shared_ptr<ntwk::TcpPublisher<CompressionPolicy>> publisher, Socket *socket,
                        std::shared_ptr<const flatbuffers::DetachedBuffer> msg,
                        unsigned int totalMsgBytesTransferred);

    static void receiveMsgControl(std::shared_ptr<TcpPublisher<CompressionPolicy>> publisher, Socket *socket,
                                  std::unique_ptr<std_msgs::MessageControl> msgCtrl,
                                  unsigned int totalMsgCtrlBytesReceived);

private:
    asio::io_context &publisherContext;
    asio::ip::tcp::acceptor socketAcceptor;

    std::list<Socket> connectedSockets;
};

} // namespace ntwk

#include "TcpPublisher_impl.h"
