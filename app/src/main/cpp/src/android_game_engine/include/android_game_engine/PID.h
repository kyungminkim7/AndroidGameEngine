#pragma once

#include <chrono>

namespace age {

class PID {
public:
    explicit PID(float kp=0.0f, float ki=0.0f, float kd=0.0f);

    float computeCorrection(float input, float target, std::chrono::duration<float> dt);

private:
    float kp;
    float ki;
    float kd;

    float totalError = 0.0f;
    float prevError = 0.0f;
};

} // namespace age
