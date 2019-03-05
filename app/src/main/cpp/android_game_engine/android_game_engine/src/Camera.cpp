#define GLM_ENABLE_EXPERIMENTAL

#include <android_game_engine/Camera.h>

#include <glm/gtx/norm.hpp>

namespace age {
Camera::Camera(float fov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane)
        : GameObject(),
          fov_rad(glm::radians(fov_deg)), aspectRatioWidthToHeight(aspectRatioWidthToHeight),
          nearPlane(nearPlane), farPlane(farPlane), linearSpeed(10.0f),
          horizontalRotationAxis(0.0f, 0.0f, 1.0f) {
    this->updateProjectionMatrix();
}

void Camera::onUpdate(std::chrono::duration<float> updateDuration) {
    if (glm::length2(this->linearVelocity) > 0.0f) {
        this->translateInLocalFrame(this->linearVelocity * updateDuration.count());
    }
}

void Camera::setFov_deg(float fov_deg) {
    this->fov_rad = glm::radians(fov_deg);
    this->updateProjectionMatrix();
}

float Camera::getFov_deg() const {
    return glm::degrees(this->fov_rad);
}

void Camera::setAspectRatioWidthToHeight(float aspectRatioWidthToHeight) {
    this->aspectRatioWidthToHeight = aspectRatioWidthToHeight;
    this->updateProjectionMatrix();
}

void Camera::updateProjectionMatrix() {
    this->projectionMatrix = glm::perspective(this->fov_rad,
                                              this->aspectRatioWidthToHeight,
                                              this->nearPlane, this->farPlane);
}

void Camera::setHorizontalRotationAxis(const glm::vec3 &horizontalRotationAxis) {
    this->horizontalRotationAxis = horizontalRotationAxis;
}

glm::vec3 Camera::getHorizontalRotationAxis() const {
    return this->horizontalRotationAxis;
}

} // namespace age