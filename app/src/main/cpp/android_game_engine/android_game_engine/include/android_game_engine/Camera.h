#pragma once

#include "GameObject.h"

namespace age {

///
/// \brief Represents a Camera.
///
class Camera : public GameObject {
public:
    ///
    /// \brief Camera constructor.
    ///
    /// Sets the following default values:
    ///     1. movement speed     = 10.0 m/s
    ///     2. rotation axis      = {0, 0, 1}
    ///     3. look at direction  = {1, 0, 0}
    ///     4. normal direction   = {0, 0, 1}
    ///
    /// \param fov_deg Camera field of view in degrees.
    /// \param aspectRatioWidthToHeight
    /// \param nearPlane Distance to camera near plane in m.
    /// \param farPlane Distance to camera far plane in m.
    ///
    Camera(float fov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane);
    
    void onUpdate(std::chrono::duration<float> updateDuration) override;
    
    void setFov_deg(float fov_deg);
    float getFov_deg() const;
    
    void setAspectRatioWidthToHeight(float aspectRatioWidthToHeight);
    
    glm::mat4 getProjectionMatrix() const;

    void setLinearSpeed(float linearSpeed);
    float getLinearSpeed() const;
    
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void stopMoving();
    
    void setHorizontalRotationAxis(const glm::vec3& horizontalRotationAxis);
    glm::vec3 getHorizontalRotationAxis() const;

private:
    void updateProjectionMatrix();
    
    float fov_rad;
    float aspectRatioWidthToHeight;
    float nearPlane; ///< m
    float farPlane;  ///< m
    
    float linearSpeed; ///< Speed of each linear velocity component (m/s)
    glm::vec3 linearVelocity {0.0f};
    
    glm::vec3 horizontalRotationAxis;
    
    glm::mat4 projectionMatrix {1.0f};
};

inline glm::mat4 Camera::getProjectionMatrix() const {return  this->projectionMatrix;}

inline void Camera::setLinearSpeed(float linearSpeed) {
    this->linearSpeed = linearSpeed > 0.0f ? linearSpeed : 0.0f;
}

inline float Camera::getLinearSpeed() const {return this->linearSpeed;}

inline void Camera::moveForward() {this->linearVelocity.x = this->linearSpeed;}
inline void Camera::moveBackward() {this->linearVelocity.x = -this->linearSpeed;}
inline void Camera::moveLeft() {this->linearVelocity.y = this->linearSpeed;}
inline void Camera::moveRight() {this->linearVelocity.y = -this->linearSpeed;}
inline void Camera::stopMoving() {this->linearVelocity = glm::vec3(0.0f);}

} // namespace age