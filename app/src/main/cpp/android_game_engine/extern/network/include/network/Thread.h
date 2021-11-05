#pragma once

#include <memory>
#include <thread>
#include <utility>

#include <asio/io_context.hpp>
#include <asio/post.hpp>

namespace ntwk {

class Thread {
private:
    using ContextPtr = std::shared_ptr<asio::io_context>;

public:
    explicit Thread(ContextPtr context=std::make_shared<asio::io_context>());
    ~Thread();

    Thread(Thread &other) noexcept = delete;
    Thread &operator=(const Thread &other) = delete;

    void stop();

    template<typename Func>
    void post(Func &&f);

private:
    ContextPtr context;
    std::thread t;
};

template<typename Func>
inline void Thread::post(Func &&f) {
    asio::post(*this->context, std::forward(f));
}

} // namespace ntwk
