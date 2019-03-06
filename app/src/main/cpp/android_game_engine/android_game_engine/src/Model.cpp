#include <android_game_engine/Model.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <android_game_engine/ShaderProgram.h>

namespace age {

glm::mat4 Model::getModelMatrix() const {
    glm::mat4 modelMatrix(this->orientation);
    
    for (int i = 0; i < 3; ++i) {
        modelMatrix[3][i] = this->position[i];
    }
    
    return glm::scale(modelMatrix, this->scale);
}

glm::mat3 Model::getNormalMatrix() const {
    if (!this->normalMatrixIsValid) {
        this->normalMatrix = glm::transpose(glm::inverse(this->getModelMatrix()));
        this->normalMatrixIsValid = true;
    }
    
    return this->normalMatrix;
}

glm::mat4 Model::getViewMatrix() const {
    return glm::lookAt(this->getPosition(),
                       this->getPosition() + this->getOrientationX(),
                       this->getOrientationZ());
}

void Model::setPosition(const glm::vec3 &position) {
    this->position = position;
    this->normalMatrixIsValid = false;
}

void Model::setOrientation(const glm::mat3 &orientation) {
    this->orientation = orientation;
    this->normalMatrixIsValid = false;
}

void Model::setOrientation(const glm::vec3 &orientationX,
                             const glm::vec3 &orientationY,
                             const glm::vec3 &orientationZ) {
    this->orientation[0] = orientationX;
    this->orientation[1] = orientationY;
    this->orientation[2] = orientationZ;
    
    this->normalMatrixIsValid = false;
}

void Model::setLookAtPoint(const glm::vec3 &lookAtPoint) {
    this->setLookAtDirection(lookAtPoint - this->getPosition());
}

void Model::setLookAtDirection(const glm::vec3 &lookAtDirection) {
    auto orientationX = glm::normalize(lookAtDirection);
    auto orientationY = glm::normalize(glm::cross(this->getOrientationZ(), orientationX));
    auto orientationZ = glm::normalize(glm::cross(orientationX, orientationY));
    this->setOrientation(orientationX, orientationY, orientationZ);
}

void Model::setNormalDirection(const glm::vec3 &normalDirection) {
    auto orientationZ = glm::normalize(normalDirection);
    auto orientationY = glm::normalize(glm::cross(orientationZ, this->getOrientationX()));
    auto orientationX = glm::normalize(glm::cross(orientationY, orientationZ));
    this->setOrientation(orientationX, orientationY, normalDirection);
}

void Model::rotate(float angle_rad, const glm::vec3 &axis) {
    auto rotation = static_cast<glm::mat3>(glm::rotate(glm::mat4(1.0f), angle_rad, axis));
    this->orientation = rotation * this->orientation;
    this->normalMatrixIsValid = false;
}

void Model::translate(const glm::vec3 &translation) {
    this->position += translation;
    this->normalMatrixIsValid = false;
}

void Model::translateInLocalFrame(const glm::vec3 &translation) {
    this->position += (this->orientation * translation);
    this->normalMatrixIsValid = false;
}

void Model::setScale(const glm::vec3 &scale) {
    this->scale = scale;
    this->normalMatrixIsValid = false;
}

void Model::render(ShaderProgram *shader) {
    shader->setUniform("model", this->getModelMatrix())
            .setUniform("normal", this->getNormalMatrix());
}

} // namespace age