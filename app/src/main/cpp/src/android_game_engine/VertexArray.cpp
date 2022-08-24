#include <android_game_engine/VertexArray.h>

#include <GLES3/gl32.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/Vertex.h>

namespace {

constexpr auto positionStride = sizeof(glm::vec3);
constexpr auto normalStride = sizeof(glm::vec3);
constexpr auto textureCoordinatesStride = sizeof(glm::vec2);

} // namespace

namespace age {

VertexArray::VertexArray(const std::vector<glm::vec3> &positions,
                         const std::vector<glm::vec3> &normals,
                         const std::vector<glm::vec2> &textureCoordinates,
                         const std::vector<glm::uvec3> &indices) :
                         numIndices(indices.size() * 3) {
    const auto positionsSize_bytes = positions.size() * positionStride;
    const auto normalsSize_bytes = normals.size() * normalStride;
    const auto textureCoordinatesSize_bytes = textureCoordinates.size() * textureCoordinatesStride;

    const auto normalsOffset = positionsSize_bytes;
    const auto textureCoordinatesOffset = normalsOffset + normalsSize_bytes;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // Store vertex data
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 positionsSize_bytes + normalsSize_bytes + textureCoordinatesSize_bytes,
                 nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, positionsSize_bytes, positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, normalsOffset, normalsSize_bytes, normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, textureCoordinatesOffset,
                    textureCoordinatesSize_bytes, textureCoordinates.data());

    // Assign vertex attributes
    glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, positionStride,
                          reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0u);

    glVertexAttribPointer(1u, 3, GL_FLOAT, GL_FALSE, normalStride,
                          reinterpret_cast<GLvoid*>(normalsOffset));
    glEnableVertexAttribArray(1u);

    glVertexAttribPointer(2u, 2, GL_FLOAT, GL_FALSE, textureCoordinatesStride,
                          reinterpret_cast<GLvoid*>(textureCoordinatesOffset));
    glEnableVertexAttribArray(2u);

    // Store indices data
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3),
                 indices.data(), GL_STATIC_DRAW);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::VertexArray(const std::vector<Vertex> &vertices,
                         const std::vector<unsigned int> &indices) :
                         numIndices(indices.size()) {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
    glGenBuffers(1, &this->ebo);

    // Copy data into GPU
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // Assign vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void *>(offsetof(Vertex, textureCoordinates)));

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
}

void VertexArray::render() {
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->numIndices,
                   GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));
}

} // namespace age
