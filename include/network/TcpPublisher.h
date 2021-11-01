#pragma once

#include <list>
#include <memory>

#include <asio/ip/tcp.hpp>
#include <asio/io_context.hpp>
#include <flatbuffers/flatbuffers.h>

#include "MsgTypeId.h"

namespace ntwk {

class TcpPublisher : public std::enable_shared_from_this<TcpPublisher> {
private:
    struct Socket;
    using SocketPtr = std::unique_ptr<Socket>;

public:
    static std::shared_ptr<TcpPublisher> create(asio::io_context &publisherContext,
                                                unsigned short port);

    void publish(MsgTypeId msgTypeId, std::shared_ptr<flatbuffers::DetachedBuffer> msg);

private:
    TcpPublisher(asio::io_context &publisherContext, unsigned short port);

    void listenForConnections();

private:
    asio::io_context &publisherContext;
    asio::ip::tcp::acceptor socketAcceptor;

    std::list<SocketPtr> connectedSockets;
};

} // namespace ntwk
