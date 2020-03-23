#include <android_game_engine/Texture2D.h>

#include <array>
#include <cstdint>
#include <unordered_map>

#include <GLES3/gl32.h>
#include <glm/vec3.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <android_game_engine/Asset.h>
#include <android_game_engine/Exception.h>
#include <android_game_engine/ManagerAssets.h>

namespace {

std::unordered_map<std::string, std::weak_ptr<unsigned int>> textureIdCache;

///
/// \brief loadImageTexture Loads and caches texture data from image file.
/// \param imageFilepath Filepath to the image.
/// \return OpenGL's texture ID for the loaded texture.
/// \exception age::LoadError Failed to load image data from file.
///
std::shared_ptr<unsigned int> loadImageTexture(const std::string &imageFilepath) {
    const auto imageFilename = imageFilepath.substr(imageFilepath.find_last_of('/') + 1);
    
    // Check cache to avoid reloading
    auto textureId = textureIdCache[imageFilename].lock();
    if (textureId) return textureId;
    
    // Load image from file
    int width, height, numChannels;
    auto asset = age::ManagerAssets::openAsset(imageFilepath);
    auto length = asset.getLength();
    std::unique_ptr<stbi_uc[]> buffer(new stbi_uc[length]);
    asset.read(buffer.get(), length);
    
    stbi_set_flip_vertically_on_load(true);
    auto img = stbi_load_from_memory(buffer.get(), length, &width, &height, &numChannels, 0);
    
    if (!img) {
        stbi_image_free(img);
        throw age::LoadError("Failed to load texture at: " + imageFilepath);
    }
    
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
    
    // Clean up texture img on GPU and clear cache
    auto textureIdDeleter = [imageFilename](auto textureId) {
        glDeleteTextures(1, textureId);
        
        textureIdCache.erase(imageFilename);
        delete textureId;
    };
    textureId = std::shared_ptr<unsigned int>(new unsigned int, textureIdDeleter);
    
    // Load texture img onto GPU
    glGenTextures(1, textureId.get());
    glBindTexture(GL_TEXTURE_2D, *textureId);
    
    glTexImage2D(GL_TEXTURE_2D,
                 0, static_cast<GLint>(format), width, height, 0,
                 format, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    textureIdCache[imageFilename] = textureId;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(img);
    return textureId;
}

} // namespace

namespace age {

Texture2D::Texture2D(const std::string &imageFilepath)
        : id(loadImageTexture(imageFilepath)) {}
        
Texture2D::Texture2D(const glm::vec3 &color) {
    std::array<uint8_t, 3> rgb{static_cast<uint8_t>(color.r * 255),
                               static_cast<uint8_t>(color.g * 255),
                               static_cast<uint8_t>(color.b * 255)};
    
    auto textureIdDeleter = [](auto textureId) {
        glDeleteTextures(1, textureId);
        delete textureId;
    };
    this->id = std::shared_ptr<unsigned int>(new unsigned int, textureIdDeleter);
    
    // Load texture img onto GPU
    glGenTextures(1, this->id.get());
    glBindTexture(GL_TEXTURE_2D, *this->id);
    
    glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, rgb.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::bind() {
    glBindTexture(GL_TEXTURE_2D, *this->id);
}

} // namespace age
