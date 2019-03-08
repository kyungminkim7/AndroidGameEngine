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
    /// \param fov_deg Camera field of view in degrees.
    /// \param aspectRatioWidthToHeight
    /// \param nearPlane Distance to camera near plane.
    /// \param farPlane Distance to camera far plane.
    ///
    Camera(float fov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane);
    
    void setFov_deg(float fov_deg);
    float getFov_deg() const;
    
    void setAspectRatioWidthToHeight(float aspectRatioWidthToHeight);
    
    glm::mat4 getProjectionMatrix() const;

private:
    void updateProjectionMatrix();
    
    float fov_rad;
    float aspectRatioWidthToHeight;
    float nearPlane; ///< m
    float farPlane;  ///< m
    
    glm::mat4 projectionMatrix {1.0f};
};

inline glm::mat4 Camera::getProjectionMatrix() const {return this->projectionMatrix;}

} // namespace age