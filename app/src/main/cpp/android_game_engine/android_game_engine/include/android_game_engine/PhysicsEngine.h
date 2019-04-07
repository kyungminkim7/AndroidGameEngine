#pragma once

#include <chrono>

namespace age {

class PhysicsRigidBody;

class PhysicsEngine {
public:
    virtual ~PhysicsEngine() = default;
    
    virtual void onUpdate(std::chrono::duration<float> updateDuration) = 0;
    
    virtual void addRigidBody(PhysicsRigidBody *body) = 0;
};

} // namespace age