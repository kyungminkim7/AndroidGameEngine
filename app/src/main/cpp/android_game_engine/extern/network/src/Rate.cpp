#include <network/Rate.h>

#include <thread>

namespace ntwk {

Rate::Rate(unsigned int fps) : period(1.0f / static_cast<float>(fps)),
    lastUpdateTime(std::chrono::steady_clock::now()) {  }

void Rate::sleep() {
    const auto currentTime = std::chrono::steady_clock::now();
    const auto timeElapsed = currentTime - this->lastUpdateTime;
    if (timeElapsed < this->period) {
        std::this_thread::sleep_for(this->period - timeElapsed);
    }
    this->lastUpdateTime = currentTime;
}

} // namespace ntwk
