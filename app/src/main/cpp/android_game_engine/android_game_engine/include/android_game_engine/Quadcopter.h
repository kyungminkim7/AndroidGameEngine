#pragma once

#include "GameObject.h"

namespace age {

class Quadcopter : public GameObject {
public:
    explicit Quadcopter(const std::string &modelFilepath);
    
    void onUpdate(std::chrono::duration<float> updateDuration) override;
    
    void onThrottleInput(const glm::vec2 &input);
    void onControlInput(const glm::vec2 &input);
    
    void setMaxMotorThrust(float thrust);
    void setMaxMotorPercentDiff(float percent);
    void setThrustMomentRatio(float thrustMomentRatio);
    
private:
    float maxMotorThrust = 0.0f;
    float maxMotorPercentDiff;
    float thrustMomentRatio;
    
    float frontLeftThrust = 0.0f;
    float frontRightThrust = 0.0f;
    float backRightThrust = 0.0f;
    float backLeftThrust = 0.0f;
};

} // namespace age