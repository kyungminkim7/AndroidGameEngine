#define GLM_ENABLE_EXPERIMENTAL

#include <android_game_engine/CameraFPV.h>

#include <glm/trigonometric.hpp>

namespace age {

CameraFPV::CameraFPV(float maxFov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane)
        : Camera(maxFov_deg, aspectRatioWidthToHeight, nearPlane, farPlane),
        rotationSensitivity(0.2f){}

bool CameraFPV::onMotionDown(const age::MotionEvent &event) {
    auto motion = event.begin();
    this->rotationMotionId = motion->first;
    this->lastMotionPosition = motion->second;
    return true;
}

bool CameraFPV::onMotionMove(const age::MotionEvent &event) {
    auto rotationMotion = event.find(this->rotationMotionId);
    if (rotationMotion != event.end()) {
        auto offset = rotationMotion->second - this->lastMotionPosition;
        auto deltaYaw = static_cast<float>(glm::radians(-offset.x * this->rotationSensitivity));
        auto deltaPitch = static_cast<float>(glm::radians(offset.y * this->rotationSensitivity));
    
        this->rotate(deltaPitch, this->getOrientationY());
        this->rotate(deltaYaw, this->getHorizontalRotationAxis());
        
        this->lastMotionPosition = rotationMotion->second;
    }
    
    if ((rotationMotion != event.end() && event.size() > 1) ||
        (rotationMotion == event.end() && !event.empty())) {
        this->moveForward();
    } else {
        this->stopMoving();
    }
    
    return true;
}

bool CameraFPV::onMotionUp(const age::MotionEvent &event) {
    this->stopMoving();
    return true;
}

} // namespace age