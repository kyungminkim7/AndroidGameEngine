#define GLM_ENABLE_EXPERIMENTAL

#include <android_game_engine/CameraFPV.h>

#include <glm/gtx/norm.hpp>
#include <glm/trigonometric.hpp>

namespace age {

CameraFPV::CameraFPV(float fov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane)
        : Camera(fov_deg, aspectRatioWidthToHeight, nearPlane, farPlane),
        horizontalRotationAxis(0.0f, 0.0f, 1.0f), linearSpeed(10.0f),
        pitchSpeed_rad(glm::radians(50.0f)), yawSpeed_rad(glm::radians(50.0f)) {}

void CameraFPV::onUpdate(std::chrono::duration<float> updateDuration) {
    // Rotation
    if (glm::length2(glm::vec2(this->pitchVelocity_rad, this->yawVelocity_rad)) > 0.0f) {
        auto deltaYaw = -this->yawVelocity_rad * updateDuration.count();
        auto deltaPitch = this->pitchVelocity_rad * updateDuration.count();
    
        this->rotate(deltaPitch, this->getOrientationY());
        this->rotate(deltaYaw, this->horizontalRotationAxis);
    }
    
    // Translation
    if (glm::length2(this->linearVelocity) > 0.0f) {
        this->translateInLocalFrame(this->linearVelocity * updateDuration.count());
    }
}

void CameraFPV::onMove(const glm::vec2 &input) {
    this->linearVelocity = {input[1] * this->linearSpeed,
                            -input[0] * this->linearSpeed,
                            0.0f};
}

void CameraFPV::onRotate(const glm::vec2 &input) {
    this->pitchVelocity_rad = -input[1] * this->pitchSpeed_rad;
    this->yawVelocity_rad = input[0] * this->yawSpeed_rad;
}

void CameraFPV::setHorizontalRotationAxis(const glm::vec3 &horizontalRotationAxis) {
    this->horizontalRotationAxis = horizontalRotationAxis;
}

void CameraFPV::setLinearSpeed(float speed) {this->linearSpeed = speed;}
void CameraFPV::setPitchSpeed(float speed_deg) {this->pitchSpeed_rad = glm::radians(speed_deg);}
void CameraFPV::setYawSpeed(float speed_deg) {this->yawSpeed_rad = glm::radians(speed_deg);}

} // namespace age