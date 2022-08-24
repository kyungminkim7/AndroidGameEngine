#pragma once

#include <BulletCollision/CollisionShapes/btCompoundShape.h>

#include <memory>
#include <vector>

namespace age {

///
/// \brief Wrapper class for a compound physics shape that packages and
///        takes ownership of child collision shapes.
///
class PhysicsCompoundShape : public btCompoundShape {
public:
    explicit PhysicsCompoundShape(int initialChildCapacity = 0);
    
    void addChild(std::unique_ptr<btCollisionShape> child,
                  const btTransform& localTransform=btTransform::getIdentity());

private:
    std::vector<std::unique_ptr<btCollisionShape>> children;
};

} // namespace age