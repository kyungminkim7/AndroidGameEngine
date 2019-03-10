#pragma once

#include <vector>

namespace age {

class ShaderProgram;

///
/// \brief Wrapper class for OpenGL Vertex Buffer Object.
///
class VertexBufferObject {
public:
    VertexBufferObject(const std::vector<float> &positions,
                       const std::vector<float> &normals,
                       const std::vector<float> &textureCoords);
    ~VertexBufferObject();
    
    void bind(ShaderProgram *shader);

private:
    unsigned int vbo;
    
    size_t normalsOffset;
    size_t textureCoordsOffset;
};

} // namespace age