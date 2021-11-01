#include <network/Rate.h>

#include <thread>

namespace ntwk {

Rate::Rate(unsigned int fps) : period(1.0f / static_cast<float>(fps)),
    lastUpdateTime(std::chrono::steady_clock::now()) {  }

void Rate::sleep() {
    std::this_thread::sleep_until(this->lastUpdateTime + this->period);
    this->lastUpdateTime = std::chrono::steady_clock::now();
}

} // namespace ntwk
