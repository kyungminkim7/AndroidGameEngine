#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <forward_list>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include <asio/io_context.hpp>

#include "Compression.h"

namespace ntwk {

struct Image;
class TcpPublisher;
class TcpSubscriber;

class Node {
public:
    explicit Node(unsigned short fps = 60);
    ~Node();

    std::shared_ptr<TcpPublisher> advertise(unsigned short port,
                                            unsigned int msgQueueSize=1,
                                            Compression compression=Compression::NONE);
    std::shared_ptr<TcpSubscriber> subscribe(const std::string &host, unsigned short port,
                                             std::function<void(std::unique_ptr<uint8_t[]>)> msgReceivedHandler,
                                             unsigned int msgQueueSize=1,
                                             Compression Compression=Compression::NONE);
    std::shared_ptr<TcpSubscriber> subscribe(const std::string &host, unsigned short port,
                                             std::function<void(std::unique_ptr<Image>)> imgMsgReceivedHandler,
                                             unsigned int msgQueueSize=1,
                                             Compression Compression=Compression::NONE);

    void update();
    void sleep();

private:
    asio::io_context ioContext;
    std::chrono::duration<float> period;
    std::chrono::system_clock::time_point lastUpdateTime;
    bool lastUpdateTimeInitialized = false;

    std::forward_list<std::shared_ptr<TcpPublisher>> publishers;
    std::mutex publishersMutex;

    std::forward_list<std::shared_ptr<TcpSubscriber>> subscribers;
    std::mutex subscribersMutex;

    std::atomic<bool> running;
    std::thread updateThread;
};

} // namespace ntwk
