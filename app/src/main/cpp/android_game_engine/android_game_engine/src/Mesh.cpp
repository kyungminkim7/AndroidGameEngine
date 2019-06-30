#include <android_game_engine/Mesh.h>

#include <GLES3/gl32.h>
#include <glm/vec3.hpp>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/ShaderProgram.h>
#include <android_game_engine/VertexBufferObject.h>

namespace age {

Mesh::Mesh(std::shared_ptr<age::VertexBufferObject> vbo,
           std::shared_ptr<age::ElementBufferObject> ebo,
           const std::set<std::string> &diffuseTextureFilepaths,
           const std::set<std::string> &specularTextureFilepaths)
           : vbo(std::move(vbo)), ebo(std::move(ebo)) {
    for (const auto& file : diffuseTextureFilepaths) {
        this->diffuseTextures.emplace_back(file);
    }
    
    for (const auto& file : specularTextureFilepaths) {
        this->specularTextures.emplace_back(file);
    }
    
    this->init();
}

Mesh::Mesh(std::shared_ptr<VertexBufferObject> vbo, std::shared_ptr<ElementBufferObject> ebo,
           const std::vector<Texture2D> &diffuseTextures,
           const std::vector<Texture2D> &specularTextures)
           : vbo(std::move(vbo)), ebo(std::move(ebo)),
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
    this->vbo->bind(shader);
    this->ebo->bind();
    this->bindTextures(shader);
    glDrawElements(GL_TRIANGLES, this->ebo->getNumIndices(),
                   GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));
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
