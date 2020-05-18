#include <network/Node.h>

#include <network/TcpPublisher.h>
#include <network/TcpSubscriber.h>

namespace ntwk {

Node::Node() : running(true), updateThread([this]{
    while (this->running.load()) {
        {
            std::lock_guard<std::mutex> guard(this->publishersMutex);
            for (auto &p : this->publishers) {
                p->update();
            }
        }

        {
            std::lock_guard<std::mutex> guard(this->subscribersMutex);
            for (auto &s : this->subscribers) {
                s->update();
            }
        }
    }
}) { }

Node::~Node() {
    this->running.store(false);
    this->updateThread.join();
}

std::shared_ptr<TcpPublisher> Node::advertise(unsigned short port,
                                              unsigned int msgQueueSize,
                                              Compression compression) {
    auto p = TcpPublisher::create(this->ioContext, port, msgQueueSize, compression);
    {
        std::lock_guard<std::mutex> guard(this->publishersMutex);
        this->publishers.emplace_front(p);
    }
    return p;
}

std::shared_ptr<TcpSubscriber> Node::subscribe(const std::string &host, unsigned short port,
                                               std::function<void (std::unique_ptr<uint8_t[]>)> msgReceivedHandler,
                                               unsigned int msgQueueSize, Compression compression) {
    auto s = TcpSubscriber::create(this->ioContext, host, port,
                                   std::move(msgReceivedHandler),
                                   msgQueueSize, compression);
    {
        std::lock_guard<std::mutex> guard(this->subscribersMutex);
        this->subscribers.emplace_front(s);
    }
    return s;
}

std::shared_ptr<TcpSubscriber> Node::subscribe(const std::string &host, unsigned short port,
                                               std::function<void (std::unique_ptr<Image>)> imgMsgReceivedHandler,
                                               unsigned int msgQueueSize, Compression compression) {
    auto s = TcpSubscriber::create(this->ioContext, host, port,
                                   std::move(imgMsgReceivedHandler),
                                   msgQueueSize, compression);
    {
        std::lock_guard<std::mutex> guard(this->subscribersMutex);
        this->subscribers.emplace_front(s);
    }
    return s;
}

void Node::update() {
    this->ioContext.poll();
    this->ioContext.restart();
}

} // namespace ntwk
