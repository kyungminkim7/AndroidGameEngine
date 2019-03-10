#pragma once

#include <string>
#include <vector>

//#include <assimp/material.h>
//#include <assimp/mesh.h>

#include "Texture2D.h"

namespace age {

class ElementBufferObject;
class ShaderProgram;
class VertexBufferObject;

///
/// \brief Aggregates vertex and index data to load onto the GPU
///        and render onto the screen.
///
class Mesh {
public:
    Mesh(std::shared_ptr<VertexBufferObject> vbo, std::shared_ptr<ElementBufferObject> ebo,
         const std::vector<std::string> &diffuseTextureFilepaths,
         const std::vector<std::string> &specularTextureFilepaths);
    
    void render(ShaderProgram *shader);

private:
    void bindTextures(ShaderProgram *shader);
    
    std::shared_ptr<VertexBufferObject> vbo;
    std::shared_ptr<ElementBufferObject> ebo;
    
    std::vector<Texture2D> diffuseTextures;
    std::vector<Texture2D> specularTextures;
};

} // namespace age