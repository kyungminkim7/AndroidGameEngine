#include <android_game_engine/Vehicle.h>

#include <glm/gtx/norm.hpp>

namespace age {

Vehicle::Vehicle(const std::string &modelFilepath, float maxEngineForce, float maxSteeringForce) :
                 GameObject(modelFilepath),
                 maxEngineForce(maxEngineForce), maxSteeringForce(maxSteeringForce),
                 currentEngineForce(0.0f), currentSteeringForce(0.0f) {}

void Vehicle::onUpdate(std::chrono::duration<float> updateDuration) {
    GameObject::onUpdate(updateDuration);

    if (std::abs(this->currentEngineForce) < 0.001f && glm::length2(this->getLinearVelocity()) < 0.001f) {
        return;
    }

    this->applyCentralForce(this->getLookAtDirection() * this->currentEngineForce);
    this->applyTorque(this->getNormalDirection() * this->currentSteeringForce *
                      this->getScaledDimensions().x * 0.5f);
}

void Vehicle::onJoystickInput(const glm::vec2 &input) {
    this->currentSteeringForce = -input.x * this->maxSteeringForce;
    this->currentEngineForce = input.y * this->maxEngineForce;
}

} // namespace age