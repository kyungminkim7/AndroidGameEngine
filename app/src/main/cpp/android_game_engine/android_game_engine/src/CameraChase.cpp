#include <android_game_engine/CameraChase.h>

#include <android_game_engine/GameObject.h>

namespace age {

CameraChase::CameraChase(float fov_deg, float aspectRatioWidthToHeight,
                         float nearPlane, float farPlane)
    : Camera(fov_deg, aspectRatioWidthToHeight, nearPlane, farPlane),
    chaseVelocityRatio(0.4f) {}

void CameraChase::onUpdate(std::chrono::duration<float> updateDuration) {
    if (!this->chaseObject) return;
    
    auto objPosition = this->chaseObject->getPosition();
    auto offset = this->chaseObject->getOrientation() * this->chaseRelativeOffset;
    this->translate((objPosition + offset - this->getPosition()) *
                    this->chaseVelocityRatio);
    
    this->setLookAtPoint(objPosition);
    this->setNormalDirection(this->chaseObject->getNormalDirection());
//    this->setNormalDirection({0.0f, 0.0f, 1.0f});
}

void CameraChase::setChaseObject(age::GameObject *object, const glm::vec3 &relativeOffset) {
    this->chaseObject = object;
    this->chaseRelativeOffset = relativeOffset;
}

} // namespace age