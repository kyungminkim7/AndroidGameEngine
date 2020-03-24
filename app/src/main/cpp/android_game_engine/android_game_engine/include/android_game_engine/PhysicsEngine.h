#pragma once

#include <chrono>
#include <memory>

#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/CollisionDispatch/btCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <glm/vec3.hpp>

namespace age {

class GameObject;
class PhysicsDebugDrawer;
class PhysicsRigidBody;
class ShaderProgram;

struct RaycastResult {
    GameObject *gameObject;
    glm::vec3 hitPoint;
    glm::vec3 hitNormal;
};

///
/// \brief Encapsulates physics and collision detection behavior.
///
class PhysicsEngine{
public:
    ///
    /// Construct PhysicsEngine.
    /// \param debugShader Shader for drawing debug objects such as collision
    ///                    objects and bounding boxes.
    ///
    explicit PhysicsEngine(ShaderProgram *debugShader);
    ~PhysicsEngine();

    PhysicsEngine(const PhysicsEngine&) = delete;
    PhysicsEngine& operator=(const PhysicsEngine&) = delete;

    void* getNativePhysicsWorld();
    
    void onUpdate(std::chrono::duration<float> updateDuration);

    void setGravity(const glm::vec3 &gravity);

    ///
    /// Add a rigid body for physics simulation and collision detection
    /// \param body
    ///
    void addRigidBody(PhysicsRigidBody *body);

    void removeRigidBody(PhysicsRigidBody *body);
    
    RaycastResult raycastClosest(const glm::vec3 &from, const glm::vec3 &to) const;
    
    ///
    /// Draw registered physics body collision objects and bounding boxes.
    ///
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