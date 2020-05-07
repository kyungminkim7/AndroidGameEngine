#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <asio/ip/tcp.hpp>

namespace ntwk {

class TcpSubscriber {
public:
    using MessageReceivedHandler = std::function<void(std::unique_ptr<uint8_t[]>)>;

    static std::shared_ptr<TcpSubscriber> create(asio::io_context &ioContext,
                                                 const std::string &host,
                                                 unsigned short port,
                                                 MessageReceivedHandler msgReceivedHandler);

private:
    TcpSubscriber(asio::io_context &ioContext,
                  const std::string &host, unsigned short port,
                  MessageReceivedHandler msgReceivedHandler);

    void connect(std::shared_ptr<TcpSubscriber> subscriber);
    void receiveMsgHeader(std::shared_ptr<TcpSubscriber> subscriber);
    void receiveMsg(std::shared_ptr<TcpSubscriber> subscriber, std::size_t msgSize_bytes);

    asio::ip::tcp::socket socket;
    asio::ip::tcp::endpoint endpoint;

    MessageReceivedHandler msgReceivedHandler;
};

} // namespace ntwk
