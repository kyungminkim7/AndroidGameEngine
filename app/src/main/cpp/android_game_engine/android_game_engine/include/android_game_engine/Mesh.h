#pragma once

#include <string>
#include <vector>

//#include <assimp/material.h>
//#include <assimp/mesh.h>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/VertexBufferObject.h>

namespace age {

class ShaderProgram;
class Texture2D;

///
/// \brief Aggregates vertex and index data to load onto the GPU
///        and render onto the screen.
///
class Mesh {
public:
    Mesh(const std::vector<float> &vertices, const std::vector<float> &normals,
         const std::vector<float> &textureCoords, const std::vector<unsigned int> &indices,
         const std::string &textureFilepath=std::string());
    
    void render(ShaderProgram *shader);

private:
    void bindTextures(ShaderProgram *shader);
    
    VertexBufferObject vbo;
    ElementBufferObject ebo;
    
    std::vector<Texture2D> ambientTextures;
    std::vector<Texture2D> diffuseTextures;
    std::vector<Texture2D> specularTextures;
};

} // namespace age