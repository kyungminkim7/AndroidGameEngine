#pragma once

#include <cstdint>
#include <forward_list>
#include <functional>
#include <memory>
#include <thread>

#include <asio/io_context.hpp>

#include "Compression.h"

namespace ntwk {

struct Image;
class TcpPublisher;
class TcpSubscriber;

class Node {
public:
    Node();
    ~Node();

    std::shared_ptr<TcpPublisher> advertise(unsigned short port,
                                            Compression compression=Compression::NONE);
    std::shared_ptr<TcpSubscriber> subscribe(const std::string &host, unsigned short port,
                                             std::function<void(std::unique_ptr<uint8_t[]>)> msgReceivedHandler,
                                             Compression Compression=Compression::NONE);
    std::shared_ptr<TcpSubscriber> subscribe(const std::string &host, unsigned short port,
                                             std::function<void(std::unique_ptr<Image>)> imgMsgReceivedHandler,
                                             Compression Compression=Compression::NONE);

    void run();
    void runOnce();

private:
    asio::io_context mainContext;
    asio::io_context tasksContext;

    std::forward_list<std::shared_ptr<TcpPublisher>> publishers;
    std::forward_list<std::shared_ptr<TcpSubscriber>> subscribers;

    std::thread tasksThread;
};

} // namespace ntwk
