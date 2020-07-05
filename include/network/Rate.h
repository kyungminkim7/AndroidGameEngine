#pragma once

#include <chrono>

namespace ntwk {

class Rate {
public:
    explicit Rate(unsigned int fps);
    void sleep();

private:
    std::chrono::duration<float> period;
    std::chrono::steady_clock::time_point lastUpdateTime;
};

} // namespace ntwk
