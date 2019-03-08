#pragma once

#include "Camera.h"

#include <glm/vec2.hpp>

#include "TouchEvent.h"

namespace age {

///
/// \brief First person shooter camera with a perspective view.
///
/// This camera pitches and yaws but does not allow roll.
///
class CameraFPV : public Camera {
public:
    CameraFPV(float maxFov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane);
    
    bool onMotionDown(const TouchEvent &event);
    bool onMotionMove(const TouchEvent &event);
    bool onMotionUp(const TouchEvent &event);
    
private:
    int rotationMotionId;
    glm::vec2 lastMotionPosition;
    float rotationSensitivity;
};

} // namespace age