#include <android_game_engine/Skybox.h>

#include <array>
#include <vector>

#include <GLES3/gl32.h>
#include <stb_image.h>

#include <android_game_engine/Exception.h>
#include <android_game_engine/ManagerAssets.h>
#include <android_game_engine/ShaderProgram.h>

namespace {

unsigned int loadCubemapTexture(const std::array<std::string, 6> &imageFilepaths) {
    unsigned int texture;
    glGenTextures(1, &texture);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    
    int width, height, numChannels;
    for (auto i = 0u; i < imageFilepaths.size(); ++i) {
        auto asset = age::ManagerAssets::openAsset(imageFilepaths[i]);
        auto length = asset.getLength();
        std::unique_ptr<stbi_uc[]> buffer(new stbi_uc[length]);
        asset.read(buffer.get(), length);
        
        auto img = stbi_load_from_memory(buffer.get(), length, &width, &height, &numChannels, 0);
    
        GLenum format;
        switch (numChannels) {
            case 1:
                format = GL_ALPHA;
                break;
        
            case 3:
                format = GL_RGB;
                break;
        
            case 4:
                format = GL_RGBA;
                break;
        
            default:
                format = GL_RGB;
                break;
        }
        
        if (img) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height,
                         0, format, GL_UNSIGNED_BYTE, img);
            stbi_image_free(img);
        } else {
            stbi_image_free(img);
            glDeleteTextures(1, &texture);
            throw age::LoadError("Failed to load skybox texture: " + imageFilepaths[i]);
        }
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    return texture;
}

} // namespace

namespace age {

Skybox::Skybox(const std::array<std::string, 6> &imageFilepaths) {
    const std::vector<float> positions {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    this->texture = loadCubemapTexture(imageFilepaths);

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size() * sizeof(float)),
                 positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0u);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Skybox::~Skybox() {
    glDeleteTextures(1, &this->texture);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Skybox::render(ShaderProgram *shader) {
    glBindVertexArray(this->vao);
    
    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("skybox", 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

} // namespace age
