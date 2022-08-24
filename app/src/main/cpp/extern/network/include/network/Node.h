#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <utility>

#include <asio/io_context.hpp>
#include <flatbuffers/flatbuffers.h>

#include "MsgTypeId.h"
#include "Thread.h"

namespace ntwk {

class TcpPublisher;
class TcpSubscriber;

class Node {
private:
    using Endpoint = std::pair<std::string, unsigned short>;
    using ContextPtr = std::shared_ptr<asio::io_context>;
    using PublisherPtr = std::shared_ptr<TcpPublisher>;
    using SubscriberPtr = std::shared_ptr<TcpSubscriber>;
    using MsgHandler = std::function<void(std::unique_ptr<uint8_t[]> &&)>;

public:

    explicit Node(ContextPtr context=std::make_shared<asio::io_context>());
    ~Node();

    void advertise(unsigned short port);
    void subscribe(const Endpoint &endpoint, MsgTypeId msgTypeId, MsgHandler msgHandler);

    void publish(MsgTypeId msgTypeId, std::shared_ptr<flatbuffers::DetachedBuffer> msg);

    void run();
    void runOnce();

private:
    ContextPtr mainContext;
    ContextPtr ntwkContext;

    std::map<Endpoint, SubscriberPtr> subscribers;
    PublisherPtr publisher;

    Thread ntwkThread;
};

} // namespace ntwk
