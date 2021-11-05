#include <network/Node.h>

#include <network/TcpPublisher.h>
#include <network/TcpSubscriber.h>

namespace ntwk {

Node::Node(ContextPtr context) :
    mainContext(std::move(context)),
    ntwkContext(std::make_shared<asio::io_context>()),
    ntwkThread(ntwkContext) { }

Node::~Node() {
    this->mainContext->stop();
}

void Node::advertise(unsigned short port) {
    this->publisher = TcpPublisher::create(*this->ntwkContext, port);
}

void Node::subscribe(const Endpoint &endpoint, MsgTypeId msgType, MsgHandler msgHandler) {
    auto &s = this->subscribers[endpoint];
    if (!s) {
        s = TcpSubscriber::create(*this->mainContext, *this->ntwkContext,
                                  endpoint.first, endpoint.second);
    }
    s->subscribe(msgType, std::move(msgHandler));
}

void Node::publish(MsgTypeId msgTypeId, std::shared_ptr<flatbuffers::DetachedBuffer> msg) {
    this->publisher->publish(msgTypeId, std::move(msg));
}

void Node::run() {
    auto work = asio::make_work_guard(*this->mainContext);
    this->mainContext->run();
}

void Node::runOnce() {
    this->mainContext->poll();
    this->mainContext->restart();
}

} // namespace ntwk
