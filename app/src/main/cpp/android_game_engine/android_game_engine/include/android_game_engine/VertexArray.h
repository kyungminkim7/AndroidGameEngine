#pragma once

#include <vector>

#include <glm/fwd.hpp>

namespace age {

    ///
    /// \brief Wrapper class for OpenGL Vertex Array Object.
    ///
    class VertexArray {
    public:
        VertexArray(const std::vector<glm::vec3> &positions,
                    const std::vector<glm::vec3> &normals,
                    const std::vector<glm::vec2> &textureCoordinates,
                    const std::vector<glm::uvec3> &indices);

        ~VertexArray();

        VertexArray(VertexArray &&) = default;
        VertexArray& operator=(VertexArray &&) = default;

        void render();

    private:
        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;

        size_t numIndices;
    };

} // namespace age
