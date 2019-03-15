#pragma once

#include <vector>

#include <glm/fwd.hpp>

namespace age {

class ShaderProgram;

///
/// \brief Wrapper class for OpenGL Vertex Buffer Object.
///
class VertexBufferObject {
public:
    VertexBufferObject(const std::vector<glm::vec3> &positions,
                       const std::vector<glm::vec3> &normals,
                       const std::vector<glm::vec2> &textureCoords);
    
    ~VertexBufferObject();
    
    void bind(ShaderProgram *shader);

private:
    unsigned int vbo;
    
    size_t normalsOffset;
    size_t textureCoordsOffset;
};

} // namespace age