#include <android_game_engine/Mesh.h>

#include <GLES3/gl32.h>
#include <glm/vec3.hpp>

#include <android_game_engine/ShaderProgram.h>
#include <android_game_engine/VertexArray.h>

namespace age {

Mesh::Mesh(std::shared_ptr<age::VertexArray> vao,
           const std::set<std::string> &diffuseTextureFilepaths,
           const std::set<std::string> &specularTextureFilepaths)
        : vao(std::move(vao)) {
    for (const auto& file : diffuseTextureFilepaths) {
        this->diffuseTextures.emplace_back(file);
    }

    for (const auto& file : specularTextureFilepaths) {
        this->specularTextures.emplace_back(file);
    }

    this->init();
}

Mesh::Mesh(std::shared_ptr<VertexArray> vao,
           const std::vector<Texture2D> &diffuseTextures,
           const std::vector<Texture2D> &specularTextures)
        : vao(std::move(vao)),
          diffuseTextures(diffuseTextures), specularTextures(specularTextures) {
    this->init();
}

void Mesh::init() {
    if (this->diffuseTextures.empty()) {
        this->diffuseTextures.emplace_back(glm::vec3(1.0f));
    }
    
    if (this->specularTextures.empty()) {
        this->specularTextures.emplace_back(glm::vec3(1.0f));
    }
}

void Mesh::render(ShaderProgram *shader) {
    this->bindTextures(shader);
    this->vao->render();
}

void Mesh::bindTextures(ShaderProgram *shader) {
    int textureUnit = 0;
    
    for (size_t i = 0; i < this->diffuseTextures.size(); ++i, ++textureUnit) {
        glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(textureUnit));
        shader->setUniform("material.diffuseTexture" + std::to_string(i), textureUnit);
        this->diffuseTextures[i].bind();
    }
    
    for (size_t i = 0; i < this->specularTextures.size(); ++i, ++textureUnit) {
        glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(textureUnit));
        shader->setUniform("material.specularTexture" + std::to_string(i), textureUnit);
        this->specularTextures[i].bind();
    }
    
    glActiveTexture(GL_TEXTURE0);
}

} // namespace ge
