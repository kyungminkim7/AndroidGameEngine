#pragma once

#include <set>
#include <string>
#include <vector>

#include "Texture2D.h"

namespace age {

class ShaderProgram;
class VertexArray;

///
/// \brief Aggregates vertex and index data to load onto the GPU
///        and render onto the screen.
///
class Mesh {
public:
    Mesh(std::shared_ptr<VertexArray> vao,
         const std::set<std::string> &diffuseTextureFilepaths,
         const std::set<std::string> &specularTextureFilepaths);

    Mesh(std::shared_ptr<VertexArray> vao,
         const std::vector<Texture2D> &diffuseTextures,
         const std::vector<Texture2D> &specularTextures);

    void bindTextures(ShaderProgram *shader);
    void renderVAO(ShaderProgram *shader);

private:
    void init();

    std::shared_ptr<VertexArray> vao;

    std::vector<Texture2D> diffuseTextures;
    std::vector<Texture2D> specularTextures;
};

} // namespace age