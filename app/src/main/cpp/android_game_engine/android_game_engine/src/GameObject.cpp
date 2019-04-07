#include <android_game_engine/GameObject.h>

#include <tuple>

#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/ModelLoader3ds.h>
#include <android_game_engine/ShaderProgram.h>

namespace age {

GameObject::GameObject() : meshes(std::make_shared<Meshes>()) {}

GameObject::GameObject(const std::string &modelFilepath){
    ModelLoader3ds modelLoader(modelFilepath);
    this->meshes = std::move(modelLoader.loadMeshes());
    this->physicsBody = std::make_unique<PhysicsRigidBody>(std::move(modelLoader.loadCollisionShape()));
}

void GameObject::onUpdate(std::chrono::duration<float> updateDuration) {}

void GameObject::updateFromPhysics() {
    if (this->physicsBody && this->physicsBody->isActive()) {
        auto transform = this->physicsBody->getTransform();
        this->model.setOrientation(std::get<0>(transform));
        this->model.setPosition(std::get<1>(transform));
    }
}

void GameObject::render(ShaderProgram *shader) {
    this->model.render(shader);

    shader->setUniform("material.specularExponent", this->specularExponent);

    for (auto& mesh : *this->meshes) {
        mesh.render(shader);
    }
}

void GameObject::setMesh(std::shared_ptr<Meshes> mesh) {
    this->meshes = std::move(mesh);
}

void GameObject::setPosition(const glm::vec3 &position) {
    this->model.setPosition(position);
    
    if (this->physicsBody) {
        this->physicsBody->setPosition(position);
    }
}

void GameObject::setOrientation(const glm::mat3& orientation) {
    this->model.setOrientation(orientation);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(orientation);
    }
}

void GameObject::setOrientation(const glm::vec3 &orientationX,
                                const glm::vec3 &orientationY,
                                const glm::vec3 &orientationZ) {
    this->model.setOrientation(orientationX, orientationY, orientationZ);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::setLookAtPoint(const glm::vec3 &lookAtPoint) {
    this->model.setLookAtPoint(lookAtPoint);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::setLookAtDirection(const glm::vec3 &lookAtDirection) {
    this->model.setLookAtDirection(lookAtDirection);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::setNormalDirection(const glm::vec3 &normalDirection) {
    this->model.setNormalDirection(normalDirection);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::rotate(float angle_rad, const glm::vec3 &axis) {
    this->model.rotate(angle_rad, axis);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::translate(const glm::vec3 &translation) {
    this->model.translate(translation);
    
    if (this->physicsBody) {
        this->physicsBody->setPosition(this->model.getPosition());
    }
}

void GameObject::translateInLocalFrame(const glm::vec3 &translation) {
    this->model.translateInLocalFrame(translation);
    
    if (this->physicsBody) {
        this->physicsBody->setPosition(this->model.getPosition());
    }
}

void GameObject::setScale(const glm::vec3& scale) {
    this->model.setScale(scale);
    
    if (this->physicsBody) {
        this->physicsBody->setScale(scale);
    }
}

void GameObject::setSpecularExponent(float specularExponent) {
    this->specularExponent = specularExponent;
}

PhysicsRigidBody* GameObject::getPhysicsBody() {
    return this->physicsBody.get();
}

void GameObject::setMass(float mass) {
    if (this->physicsBody) {
        this->physicsBody->setMass(mass);
    }
}

void GameObject::setCollisionShape(std::shared_ptr<btCollisionShape> collisionShape) {
    this->physicsBody = std::make_unique<PhysicsRigidBody>(std::move(collisionShape));
}

} // namespace age