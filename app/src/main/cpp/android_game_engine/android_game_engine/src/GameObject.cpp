#include <android_game_engine/GameObject.h>

#include <android_game_engine/ModelLoader3ds.h>
#include <android_game_engine/ShaderProgram.h>

namespace age {

GameObject::GameObject() : meshes(std::make_shared<Meshes>()) {}

GameObject::GameObject(const std::string &modelFilepath){
    ModelLoader3ds modelLoader(modelFilepath);
    this->meshes = std::move(modelLoader.loadMeshes());
}

void GameObject::onUpdate(std::chrono::duration<float> updateDuration) {}

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

void GameObject::setScale(const glm::vec3& scale) {
    this->model.setScale(scale);
}

void GameObject::setSpecularExponent(float specularExponent) {
    this->specularExponent = specularExponent;
}

} // namespace age