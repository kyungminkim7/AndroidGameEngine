#include <android_game_engine/PhysicsEngineBullet.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <LinearMath/btVector3.h>

#include <android_game_engine/PhysicsRigidBody.h>

namespace age {

PhysicsEngineBullet::PhysicsEngineBullet()
    : collisionConfig(new btDefaultCollisionConfiguration),
      collisionDispatcher(new btCollisionDispatcher(this->collisionConfig.get())),
      overlappingPairs(new btDbvtBroadphase),
      constraintSolver(new btSequentialImpulseConstraintSolver),
      dynamicsWorld(new btDiscreteDynamicsWorld(this->collisionDispatcher.get(),
                                                this->overlappingPairs.get(),
                                                this->constraintSolver.get(),
                                                this->collisionConfig.get())) {
    this->dynamicsWorld->setGravity({0.0f, 0.0f, -9.80665f});
}

void PhysicsEngineBullet::onUpdate(std::chrono::duration<float> updateDuration) {
    this->dynamicsWorld->stepSimulation(updateDuration.count(), 10);
}

void PhysicsEngineBullet::addRigidBody(PhysicsRigidBody *body) {
    this->dynamicsWorld->addRigidBody(reinterpret_cast<btRigidBody*>(body->getNativeBody()));
}

} // namespace age