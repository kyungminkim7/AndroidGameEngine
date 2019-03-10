#include <android_game_engine/VertexBufferObject.h>

#include <GLES2/gl2.h>

#include <android_game_engine/ShaderProgram.h>

namespace {

constexpr auto positionStride = 3 * sizeof(float);
constexpr auto normalStride = 3 * sizeof(float);
constexpr auto textureCoordStride = 2 * sizeof(float);

} // namespace

namespace age {

VertexBufferObject::VertexBufferObject(const std::vector<float> &positions,
                                       const std::vector<float> &normals,
                                       const std::vector<float> &textureCoords) {
    const auto positionsSize_bytes = positions.size() * sizeof(float);
    const auto normalsSize_bytes = normals.size() * sizeof(float);
    const auto textureCoordsSize_bytes = textureCoords.size() * sizeof(float);
    
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