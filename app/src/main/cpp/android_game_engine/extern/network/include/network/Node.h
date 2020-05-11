#pragma once

#include <cstdint>
#include <forward_list>
#include <functional>
#include <memory>

#include <asio/io_context.hpp>

namespace ntwk {

class TcpPublisher;
class TcpSubscriber;

class Node {
public:
    std::shared_ptr<TcpPublisher> advertise(unsigned short port, unsigned int msgQueueSize);
    std::shared_ptr<TcpSubscriber> subscribe(const std::string &host, unsigned short port,
                                             unsigned int msgQueueSize,
                                             std::function<void(std::unique_ptr<uint8_t[]>)> msgReceivedHandler);

    void update();

private:
    asio::io_context ioContext;
    std::forward_list<std::shared_ptr<TcpPublisher>> publishers;
    std::forward_list<std::shared_ptr<TcpSubscriber>> subscribers;
};

} // namespace ntwk
