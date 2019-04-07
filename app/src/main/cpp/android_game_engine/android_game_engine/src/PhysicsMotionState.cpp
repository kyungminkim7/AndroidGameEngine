#include <android_game_engine/PhysicsMotionState.h>

namespace age {

PhysicsMotionState::PhysicsMotionState(const btTransform &transform,
                                       const btTransform &centerOfMassOffset)
        : transform(transform),
          centerOfMassOffset(centerOfMassOffset),
          invCenterOfMassOffset(centerOfMassOffset.inverse()) {}

void PhysicsMotionState::setCenterOfMassOffset(const btTransform &centerOfMassOffset) {
    this->centerOfMassOffset = centerOfMassOffset;
    this->invCenterOfMassOffset = centerOfMassOffset.inverse();
}

} // namespace age