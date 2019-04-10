#pragma once

#include <chrono>
#include <memory>

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <android_game_engine/PhysicsDebugDrawer.h>

namespace age {

class PhysicsRigidBody;
class ShaderProgram;

class PhysicsEngine{
public:
    explicit PhysicsEngine(ShaderProgram *debugShader);
    
    void onUpdate(std::chrono::duration<float> updateDuration);
    
    void addRigidBody(PhysicsRigidBody *body);
    
    void renderDebug();

private:
    std::unique_ptr<PhysicsDebugDrawer> debugDrawer;
    std::unique_ptr<btCollisionConfiguration> collisionConfig;
    std::unique_ptr<btCollisionDispatcher> collisionDispatcher;
    std::unique_ptr<btBroadphaseInterface> overlappingPairs;
    std::unique_ptr<btSequentialImpulseConstraintSolver> constraintSolver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
};

} // namespace age