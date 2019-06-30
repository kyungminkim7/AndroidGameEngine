#include <android_game_engine/VertexBufferObject.h>

#include <GLES3/gl32.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/ShaderProgram.h>

namespace {

constexpr auto positionStride = 3 * sizeof(float);
constexpr auto normalStride = 3 * sizeof(float);
constexpr auto textureCoordStride = 2 * sizeof(float);

} // namespace

namespace age {

VertexBufferObject::VertexBufferObject(const std::vector<glm::vec3> &positions,
                                       const std::vector<glm::vec3> &normals,
                                       const std::vector<glm::vec2> &textureCoords) {
    const auto positionsSize_bytes = positions.size() * sizeof(glm::vec3);
    const auto normalsSize_bytes = normals.size() * sizeof(glm::vec3);
    const auto textureCoordsSize_bytes = textureCoords.size() * sizeof(glm::vec2);
    
    this->normalsOffset = positionsSize_bytes;
    this->textureCoordsOffset = positionsSize_bytes + normalsSize_bytes;
    
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 positionsSize_bytes + normalsSize_bytes + textureCoordsSize_bytes,
                 nullptr, GL_STATIC_DRAW);
    
    glBufferSubData(GL_ARRAY_BUFFER, 0, positionsSize_bytes, positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, positionsSize_bytes, normalsSize_bytes, normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, positionsSize_bytes + normalsSize_bytes,
                    textureCoordsSize_bytes, textureCoords.data());
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBufferObject::~VertexBufferObject() {
    glDeleteBuffers(1, &this->vbo);
}

void VertexBufferObject::bind(ShaderProgram *shader) {
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    
    // Positions
    shader->setVertexAttribPointer("aPos", 3, GL_FLOAT, GL_FALSE, positionStride,
                                   reinterpret_cast<GLvoid*>(0));
    shader->enableVertexAttribArray("aPos");
    
    // Normals
    shader->setVertexAttribPointer("aNormal", 3, GL_FLOAT, GL_FALSE, normalStride,
                                   reinterpret_cast<GLvoid*>(this->normalsOffset));
    shader->enableVertexAttribArray("aNormal");
    
    // Texture coordinates
    shader->setVertexAttribPointer("aTexCoord", 2, GL_FLOAT, GL_FALSE, textureCoordStride,
                                   reinterpret_cast<GLvoid*>(this->textureCoordsOffset));
    shader->enableVertexAttribArray("aTexCoord");
}

} // namespace age
