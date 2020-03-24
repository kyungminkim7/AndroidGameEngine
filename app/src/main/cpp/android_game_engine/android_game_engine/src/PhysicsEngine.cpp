#include <android_game_engine/PhysicsEngine.h>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <LinearMath/btVector3.h>

#include <android_game_engine/PhysicsDebugDrawer.h>
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

PhysicsEngine::~PhysicsEngine() = default;

void* PhysicsEngine::getNativePhysicsWorld(){ return this->dynamicsWorld.get(); }

void PhysicsEngine::onUpdate(std::chrono::duration<float> updateDuration) {
    this->dynamicsWorld->stepSimulation(updateDuration.count(), 10);
}

void PhysicsEngine::setGravity(const glm::vec3 &gravity) {
    this->dynamicsWorld->setGravity({gravity.x, gravity.y, gravity.z});
}

void PhysicsEngine::addRigidBody(PhysicsRigidBody *body) {
    this->dynamicsWorld->addRigidBody(reinterpret_cast<btRigidBody*>(body->getNativeBody()));
}

void PhysicsEngine::removeRigidBody(age::PhysicsRigidBody *body) {
    this->dynamicsWorld->removeRigidBody(reinterpret_cast<btRigidBody*>(body->getNativeBody()));
}

RaycastResult PhysicsEngine::raycastClosest(const glm::vec3 &from, const glm::vec3 &to) const {
    btVector3 btFrom(from.x, from.y, from.z), btTo(to.x, to.y, to.z);
    btCollisionWorld::ClosestRayResultCallback callback(btFrom, btTo);
    
    this->dynamicsWorld->rayTest(btFrom, btTo, callback);
    
    if(callback.hasHit()) {
        auto hitPoint = btFrom.lerp(btTo, callback.m_closestHitFraction);
        return {static_cast<GameObject*>(callback.m_collisionObject->getUserPointer()),
                glm::vec3(hitPoint.x(), hitPoint.y(), hitPoint.z()),
                glm::vec3(callback.m_hitNormalWorld.x(), callback.m_hitNormalWorld.y(), callback.m_hitNormalWorld.z())};
    } else {
        return {nullptr, glm::vec3(0.0f), glm::vec3(0.0f)};
    }
}

void PhysicsEngine::renderDebug() {
    this->dynamicsWorld->debugDrawWorld();
}

} // namespace age