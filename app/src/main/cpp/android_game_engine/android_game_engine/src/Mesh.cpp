#include <android_game_engine/Mesh.h>

#include <GLES2/gl2.h>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/ShaderProgram.h>
#include <android_game_engine/VertexBufferObject.h>

namespace {

//std::vector<ge::Texture2D> loadTexturesFromMaterial(const aiMaterial &material, aiTextureType type, const std::string &modelDirectory) {
//    std::vector<ge::Texture2D> textures;
//    textures.reserve(material.GetTextureCount(type));
//
//    for (unsigned int i = 0; i < material.GetTextureCount(type); ++i) {
//        aiString imageFilename;
//        material.GetTexture(type, i, &imageFilename);
//        textures.emplace_back(modelDirectory + "/" + imageFilename.C_Str());
//    }
//
//    return textures;
//}

} // namespace

namespace age {

Mesh::Mesh(std::shared_ptr<age::VertexBufferObject> vbo,
           std::shared_ptr<age::ElementBufferObject> ebo,
           const std::vector<std::string> &diffuseTextureFilepaths,
           const std::vector<std::string> &specularTextureFilepaths)
           : vbo(std::move(vbo)), ebo(std::move(ebo)){
    for (const auto& file : diffuseTextureFilepaths) {
        this->diffuseTextures.emplace_back(file);
    }
    
    for (const auto& file : specularTextureFilepaths) {
        this->specularTextures.emplace_back(file);
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
