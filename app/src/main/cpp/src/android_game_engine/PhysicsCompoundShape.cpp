#include <android_game_engine/PhysicsCompoundShape.h>

namespace age {

PhysicsCompoundShape::PhysicsCompoundShape(int initialChildCapacity)
    : btCompoundShape(true, initialChildCapacity) {
    this->children.reserve(initialChildCapacity);
}

void PhysicsCompoundShape::addChild(std::unique_ptr<btCollisionShape> child,
                                    const btTransform& localTransform) {
    this->addChildShape(localTransform, child.get());
    this->children.push_back(std::move(child));
}

} // namespace age