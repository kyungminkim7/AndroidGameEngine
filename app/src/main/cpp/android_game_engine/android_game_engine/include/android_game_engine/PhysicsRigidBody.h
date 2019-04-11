#pragma once

#include <memory>
#include <utility>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/PhysicsMotionState.h>

namespace age {

class PhysicsRigidBody {
public:
    explicit PhysicsRigidBody(std::unique_ptr<btCollisionShape> collisionShape);
    
    void* getNativeBody();
    
    void setOrientation(const glm::mat3 &orientation);
    void setPosition(const glm::vec3 &position);
    
    void setDamping(float linearDamping, float angularDamping);
    void setMass(float mass);
    
    void setScale(const glm::vec3 &scale);
    
    void setLinearVelocity(const glm::vec3 &velocity);
    void setAngularVelocity(const glm::vec3 &velocity);
    
    bool isActive() const;
    
    std::pair<glm::mat3, glm::vec3> getTransform() const;
    
private:
    std::unique_ptr<PhysicsMotionState> motionState;
    std::unique_ptr<btCollisionShape> collisionShape;
    std::unique_ptr<btRigidBody> body;
};

inline bool PhysicsRigidBody::isActive() const {return this->body->isActive();}

} // namespace age