#include <android_game_engine/PhysicsRigidBody.h>

namespace age {

PhysicsRigidBody::PhysicsRigidBody(std::unique_ptr<btCollisionShape> collisionShape)
    : motionState(new PhysicsMotionState),
      collisionShape(std::move(collisionShape)),
      body(new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(0.0f, this->motionState.get(),
                                                                    this->collisionShape.get()))){}

void* PhysicsRigidBody::getNativeBody() {return this->body.get();}

void PhysicsRigidBody::setOrientation(const glm::mat3 &orientation) {
    btTransform transform;
    this->motionState->getWorldTransform(transform);
    
    transform.setBasis({orientation[0][0], orientation[1][0], orientation[2][0],
                        orientation[0][1], orientation[1][1], orientation[2][1],
                        orientation[0][2], orientation[1][2], orientation[2][2]});
    
    this->motionState->setWorldTransform(transform);
    this->body->setWorldTransform(transform);
    this->body->setInterpolationWorldTransform(transform);
}

void PhysicsRigidBody::setPosition(const glm::vec3 &position) {
    btTransform transform;
    this->motionState->getWorldTransform(transform);
    transform.setOrigin({position.x, position.y, position.z});
    
    this->motionState->setWorldTransform(transform);
    this->body->setWorldTransform(transform);
    this->body->setInterpolationWorldTransform(transform);
}

void PhysicsRigidBody::setDamping(float linearDamping, float angularDamping) {
    this->body->setDamping(linearDamping, angularDamping);
}

void PhysicsRigidBody::setMass(float mass) {
    btVector3 inertia;
    this->collisionShape->calculateLocalInertia(mass, inertia);

    this->body->setMassProps(mass, inertia);
    this->body->updateInertiaTensor();
}

void PhysicsRigidBody::setScale(const glm::vec3 &scale) {
    this->collisionShape->setLocalScaling({scale.x, scale.y, scale.z});
}

void PhysicsRigidBody::setLinearVelocity(const glm::vec3 &velocity) {
    this->body->setLinearVelocity({velocity.x, velocity.y, velocity.z});
}

void PhysicsRigidBody::setAngularVelocity(const glm::vec3 &velocity) {
    this->body->setAngularVelocity({velocity.x, velocity.y, velocity.z});
}

std::pair<glm::mat3, glm::vec3> PhysicsRigidBody::getTransform() const {
    btTransform transform;
    this->motionState->getWorldTransform(transform);
    
    const auto& basis = transform.getBasis();
    const auto& origin = transform.getOrigin();
    
    return std::make_pair(glm::mat3(basis[0][0], basis[1][0], basis[2][0],
                                    basis[0][1], basis[1][1], basis[2][1],
                                    basis[0][2], basis[1][2], basis[2][2]),
                          glm::vec3(origin.getX(), origin.getY(), origin.getZ()));
}

} // namespace age