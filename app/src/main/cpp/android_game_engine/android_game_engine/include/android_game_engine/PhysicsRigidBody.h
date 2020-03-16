#pragma once

#include <memory>
#include <utility>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/PhysicsMotionState.h>

namespace age {

class GameObject;

///
/// \brief Wrapper class for a rigid physics body.
///
class PhysicsRigidBody {
public:
    PhysicsRigidBody(GameObject *gameObject, std::unique_ptr<btCollisionShape> collisionShape);
    
    void* getNativeBody();
    
    void setOrientation(const glm::mat3 &orientation);
    void setPosition(const glm::vec3 &position);
    
    void setDamping(float linearDamping, float angularDamping);

    void setMass(float mass);
    float getMass() const;
    
    void setScale(const glm::vec3 &scale);
    
    bool isActive() const;
    
    std::pair<glm::mat3, glm::vec3> getTransform() const;
    
    void applyCentralForce(const glm::vec3 &force);
    void applyTorque(const glm::vec3 &torque);
    void applyForce(const glm::vec3 &force, const glm::vec3 &relPos);
    void clearForces();
    
    void setLinearVelocity(const glm::vec3 &velocity);
    void setAngularVelocity(const glm::vec3 &velocity);

    glm::vec3 getLinearVelocity() const;
    glm::vec3 getAngularVelocity() const;

    void setFriction(float friction);
    
private:
    std::unique_ptr<PhysicsMotionState> motionState;
    std::unique_ptr<btCollisionShape> collisionShape;
    std::unique_ptr<btRigidBody> body;
};

inline float PhysicsRigidBody::getMass() const {return this->body->getMass();}

inline bool PhysicsRigidBody::isActive() const {return this->body->isActive();}

inline glm::vec3 PhysicsRigidBody::getLinearVelocity() const {
    auto velocity = this->body->getLinearVelocity();
    return {velocity.x(), velocity.y(), velocity.z()};
}

inline glm::vec3 PhysicsRigidBody::getAngularVelocity() const {
    auto velocity = this->body->getAngularVelocity();
    return {velocity.x(), velocity.y(), velocity.z()};
}

} // namespace age