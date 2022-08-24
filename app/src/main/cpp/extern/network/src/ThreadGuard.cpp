#include <network/ThreadGuard.h>

#include <stdexcept>
#include <utility>

namespace ntwk {

ThreadGuard::ThreadGuard(std::thread &&t) : t(std::move(t)) {
    if (!this->t.joinable()) {
        throw std::logic_error("Thread is not joinable");
    }
}

ThreadGuard::~ThreadGuard() {
    t.join();
}

}
