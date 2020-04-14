#include <android_game_engine/PID.h>

namespace age {

PID::PID(float kp, float ki, float kd) : kp(kp), ki(ki), kd(kd) {}

float PID::computeCorrection(float input, float target, std::chrono::duration<float> dt) {
    auto error = target - input;
    this->totalError += error;

    auto correction = this->kp * error +
            this->ki * this->totalError +
            this->kd * (error - this->prevError) / dt.count();

    this->prevError = error;

    return correction;
}

} // namespace age
