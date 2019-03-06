#pragma once

#include "Camera.h"

#include <glm/vec2.hpp>

#include "MotionEvent.h"

namespace age {

///
/// \brief First person shooter camera with a perspective view.
///
/// This camera pitches and yaws but does not allow roll.
///
class CameraFPV : public Camera {
public:
    CameraFPV(float maxFov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane);
    
    bool onMotionDown(const MotionEvent &event);
    bool onMotionMove(const MotionEvent &event);
    bool onMotionUp(const MotionEvent &event);
    
private:
    int rotationMotionId;
    glm::vec2 lastMotionPosition;
    float rotationSensitivity;
};

} // namespace age