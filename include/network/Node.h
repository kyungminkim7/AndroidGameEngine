#pragma once

#include <cstdint>
#include <functional>
#include <memory>

#include <asio/io_context.hpp>

namespace ntwk {

class TcpPublisher;
class TcpSubscriber;

class Node {
public:
    std::shared_ptr<TcpPublisher> advertise(unsigned short port);
    std::shared_ptr<TcpSubscriber> subscribe(const std::string &host, unsigned short port,
                                             std::function<void(std::unique_ptr<uint8_t[]>)> msgReceivedHandler);

    void run();
    void runOnce();

private:
    asio::io_context ioContext;
};

} // namespace ntwk
