#pragma once

#include <set>
#include <string>
#include <vector>

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
         const std::set<std::string> &diffuseTextureFilepaths,
         const std::set<std::string> &specularTextureFilepaths);
    
    Mesh(std::shared_ptr<VertexBufferObject> vbo, std::shared_ptr<ElementBufferObject> ebo,
         const std::vector<Texture2D> &diffuseTextures,
         const std::vector<Texture2D> &specularTextures);
    
    void render(ShaderProgram *shader);

private:
    void init();
    void bindTextures(ShaderProgram *shader);
    
    std::shared_ptr<VertexBufferObject> vbo;
    std::shared_ptr<ElementBufferObject> ebo;
    
    std::vector<Texture2D> diffuseTextures;
    std::vector<Texture2D> specularTextures;
};

} // namespace age