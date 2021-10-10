#pragma once

#include <vector>

#include <glm/fwd.hpp>

namespace age {

class Vertex;

///
/// \brief Wrapper class for OpenGL Vertex Array Object.
///
class VertexArray {
public:
    VertexArray(const std::vector<glm::vec3> &positions,
                const std::vector<glm::vec3> &normals,
                const std::vector<glm::vec2> &textureCoordinates,
                const std::vector<glm::uvec3> &indices);

    VertexArray(const std::vector<Vertex> &vertices,
                const std::vector<unsigned int> &indices);

    ~VertexArray();

    VertexArray(VertexArray &&) noexcept = default;
    VertexArray& operator=(VertexArray &&) noexcept = default;

    void render();

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;

    size_t numIndices;
};

} // namespace age
