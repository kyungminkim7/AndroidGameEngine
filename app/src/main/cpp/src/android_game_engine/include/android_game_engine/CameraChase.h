#pragma once

#include "Camera.h"

#include <glm/vec3.hpp>

namespace age {

class GameObject;

///
/// \brief Chase camera that drifts after a target object and remains oriented
///        according to the target object's normal.
///
class CameraChase : public Camera{
public:
    CameraChase(float fov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane);
    
    void onUpdate(std::chrono::duration<float> updateDuration) override;
    
    void setChaseObject(GameObject *object, const glm::vec3 &relativeOffset);
    
private:
    float chaseVelocityRatio;
    GameObject *chaseObject = nullptr;
    glm::vec3 chaseRelativeOffset;
};

} // namespace age