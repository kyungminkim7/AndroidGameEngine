#pragma once

#include "GameObject.h"

#include <memory>

namespace age {

class Vehicle : public GameObject {
public:
    Vehicle(const std::string &modelFilepath, float maxEngineForce, float maxSteeringForce);

    void onUpdate(std::chrono::duration<float> updateDuration) override;

    void onJoystickInput(const glm::vec2 &input);

private:
    float maxEngineForce;
    float maxSteeringForce;

    float currentEngineForce;
    float currentSteeringForce;
};

} // namespace age