#include <android_game_engine/PhysicsEngine.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <LinearMath/btVector3.h>

#include <android_game_engine/PhysicsRigidBody.h>

namespace age {

PhysicsEngine::PhysicsEngine(ShaderProgram *debugShader)
        : debugDrawer(new PhysicsDebugDrawer(debugShader)),
          collisionConfig(new btDefaultCollisionConfiguration),
          collisionDispatcher(new btCollisionDispatcher(this->collisionConfig.get())),
          overlappingPairs(new btDbvtBroadphase),
          constraintSolver(new btSequentialImpulseConstraintSolver),
          dynamicsWorld(new btDiscreteDynamicsWorld(this->collisionDispatcher.get(),
                                                    this->overlappingPairs.get(),
                                                    this->constraintSolver.get(),
                                                    this->collisionConfig.get())) {
    this->dynamicsWorld->setGravity({0.0f, 0.0f, -9.80665f});
    this->dynamicsWorld->setDebugDrawer(this->debugDrawer.get());
    this->debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
}

void PhysicsEngine::onUpdate(std::chrono::duration<float> updateDuration) {
    this->dynamicsWorld->stepSimulation(updateDuration.count(), 10);
}

void PhysicsEngine::addRigidBody(PhysicsRigidBody *body) {
    this->dynamicsWorld->addRigidBody(reinterpret_cast<btRigidBody*>(body->getNativeBody()));
}

void PhysicsEngine::renderDebug() {
    this->dynamicsWorld->debugDrawWorld();
}

} // namespace age