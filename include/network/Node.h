#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <thread>

#include <asio/io_context.hpp>

#include "Compression.h"
#include "Image.h"
#include "TcpPublisher.h"
#include "TcpSubscriber.h"

namespace ntwk {

class Node {
public:
    Node();
    ~Node();

    template<typename CompressionPolicy=Compression::IdentityPolicy>
    std::shared_ptr<TcpPublisher<CompressionPolicy>> advertise(unsigned short port);

    template<typename CompressionPolicy=Compression::Image::IdentityPolicy>
    std::shared_ptr<TcpPublisher<CompressionPolicy>> advertiseImage(unsigned short port);

    template<typename DecompressionPolicy=Compression::IdentityPolicy>
    std::shared_ptr<TcpSubscriber<uint8_t[], DecompressionPolicy>> subscribe(const std::string &host, unsigned short port,
                                                                             std::function<void(std::unique_ptr<uint8_t[]>)> msgReceivedHandler);

    template<typename DecompressionPolicy=Compression::Image::IdentityPolicy>
    std::shared_ptr<TcpSubscriber<Image, DecompressionPolicy>> subscribeImage(const std::string &host, unsigned short port,
                                                                              std::function<void(std::unique_ptr<Image>)> imgMsgReceivedHandler);

    void run();
    void runOnce();

private:
    asio::io_context mainContext;
    asio::io_context tasksContext;

    std::thread tasksThread;
};

template<typename CompressionPolicy>
std::shared_ptr<TcpPublisher<CompressionPolicy>> Node::advertise(unsigned short port) {
    return TcpPublisher<CompressionPolicy>::create(this->tasksContext, port);
}

template<typename CompressionPolicy>
std::shared_ptr<TcpPublisher<CompressionPolicy>> Node::advertiseImage(unsigned short port) {
    return TcpPublisher<CompressionPolicy>::create(this->tasksContext, port);
}

template<typename DecompressionPolicy>
std::shared_ptr<TcpSubscriber<uint8_t[], DecompressionPolicy>> Node::subscribe(const std::string &host, unsigned short port,
                                                                               std::function<void (std::unique_ptr<uint8_t[]>)> msgReceivedHandler) {
    return TcpSubscriber<uint8_t[], DecompressionPolicy>::create(this->mainContext, this->tasksContext,
                                                                 host, port, std::move(msgReceivedHandler));
}

template<typename DecompressionPolicy>
std::shared_ptr<TcpSubscriber<Image, DecompressionPolicy>> Node::subscribeImage(const std::string &host, unsigned short port,
                                                                                std::function<void (std::unique_ptr<Image>)> imgMsgReceivedHandler) {
    return TcpSubscriber<Image, DecompressionPolicy>::create(this->mainContext, this->tasksContext,
                                                             host, port, std::move(imgMsgReceivedHandler));
}

} // namespace ntwk
