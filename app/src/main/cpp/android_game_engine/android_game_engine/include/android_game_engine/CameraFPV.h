#pragma once

#include "Camera.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace age {

///
/// \brief First person shooter camera with a perspective view.
///
/// This camera pitches and yaws but does not allow roll.
///
class CameraFPV : public Camera {
public:
    CameraFPV(float fov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane);
    
    void onUpdate(std::chrono::duration<float> updateDuration) override;
    
    void onMove(const glm::vec2 &input);
    void onRotate(const glm::vec2 &input);
    
    void setHorizontalRotationAxis(const glm::vec3& horizontalRotationAxis);
    
    void setLinearSpeed(float speed);
    void setPitchSpeed(float speed_deg);
    void setYawSpeed(float speed_deg);
    
private:
    glm::vec3 horizontalRotationAxis;
    
    float linearSpeed; ///< Speed of each linear velocity component (m/s)
    glm::vec3 linearVelocity {0.0f};
    
    float pitchSpeed_rad; ///< rad/s
    float pitchVelocity_rad = 0.0f;
    
    float yawSpeed_rad; ///< rad/s
    float yawVelocity_rad = 0.0f;
};

} // namespace age