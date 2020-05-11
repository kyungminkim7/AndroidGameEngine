#include <network/Node.h>

#include <network/TcpPublisher.h>
#include <network/TcpSubscriber.h>

namespace ntwk {

std::shared_ptr<TcpPublisher> Node::advertise(unsigned short port, unsigned int msgQueueSize) {
    this->publishers.emplace_front(TcpPublisher::create(this->ioContext, port, msgQueueSize));
    return this->publishers.front();
}

std::shared_ptr<TcpSubscriber> Node::subscribe(const std::string &host, unsigned short port,
                                               unsigned int msgQueueSize,
                                               std::function<void (std::unique_ptr<uint8_t[]>)> msgReceivedHandler) {
    this->subscribers.emplace_front(TcpSubscriber::create(this->ioContext, host, port, msgQueueSize,
                                                          std::move(msgReceivedHandler)));
    return this->subscribers.front();
}

void Node::update() {
    for (auto &p : this->publishers) {
        p->update();
    }

    for (auto &s : this->subscribers) {
        s->update();
    }

    this->ioContext.poll();
    this->ioContext.restart();
}

} // namespace ntwk
