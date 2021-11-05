#include <network/Thread.h>

namespace ntwk {

Thread::Thread(ContextPtr context) : context(std::move(context)),
    t([context=this->context.get()]{
    auto work = asio::make_work_guard(*context);
    context->run();
}) { }

Thread::~Thread() {
    this->context->stop();
    t.join();
}

void Thread::stop() {
    this->context->stop();
}

} // namespace ntwk
