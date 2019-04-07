#pragma once

#include <LinearMath/btMotionState.h>

namespace age {

class PhysicsMotionState : public btMotionState {
public:
    explicit PhysicsMotionState(const btTransform& transform = btTransform::getIdentity(),
                                const btTransform& centerOfMassOffset = btTransform::getIdentity());
    
    /// Synchronizes world transform from user to physics
    void getWorldTransform(btTransform &centerOfMassWorldTrans) const override;
    
    /// Synchronizes world transform from physics to user
    /// Bullet only calls the update of worldtransform for active objects
    void setWorldTransform(const btTransform& centerOfMassWorldTrans) override;

    void setCenterOfMassOffset(const btTransform &centerOfMassOffset);
    
private:
    btTransform transform;
    btTransform centerOfMassOffset;
    btTransform invCenterOfMassOffset;
};

inline void PhysicsMotionState::getWorldTransform(btTransform &centerOfMassWorldTrans) const {
    centerOfMassWorldTrans = this->transform * this->invCenterOfMassOffset;
}

inline void PhysicsMotionState::setWorldTransform(const btTransform &centerOfMassWorldTrans) {
    this->transform = centerOfMassWorldTrans * this->centerOfMassOffset;
}

} // namespace age