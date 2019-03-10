#include <android_game_engine/DirectionalLight.h>

#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/ShaderProgram.h>

namespace age {

DirectionalLight::DirectionalLight(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                                   float left, float right, float bottom, float top, float nearPlane, float farPlane)
        : Light(ambient, diffuse, specular),
          left(left), right(right), bottom(bottom), top(top),
          nearPlane(nearPlane), farPlane(farPlane) {}

glm::mat4 DirectionalLight::getProjectionMatrix() const {
    return glm::ortho(this->left, this->right, this->bottom, this->top,
                      this->nearPlane, this->farPlane);
}

void DirectionalLight::render(ShaderProgram *shader) {
    shader->setUniform("directionalLight.direction", this->getLookAtDirection());
    shader->setUniform("directionalLight.lighting.ambient", this->getAmbient());
    shader->setUniform("directionalLight.lighting.diffuse", this->getDiffuse());
    shader->setUniform("directionalLight.lighting.specular", this->getSpecular());
}

} // namespace age