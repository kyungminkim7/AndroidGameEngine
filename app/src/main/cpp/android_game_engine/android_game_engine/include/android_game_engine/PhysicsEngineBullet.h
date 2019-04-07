#pragma once

#include "PhysicsEngine.h"

#include <memory>

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

namespace age {

class PhysicsEngineBullet : public PhysicsEngine {
public:
    PhysicsEngineBullet();
    void onUpdate(std::chrono::duration<float> updateDuration) override;
    void addRigidBody(PhysicsRigidBody *body) override;

private:
    std::unique_ptr<btCollisionConfiguration> collisionConfig;
    std::unique_ptr<btCollisionDispatcher> collisionDispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairs;
    std::unique_ptr<btSequentialImpulseConstraintSolver> constraintSolver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
};

} // namespace age