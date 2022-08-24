#pragma once

#include <memory>
#include <string>

#include <glm/fwd.hpp>

namespace age {

///
/// \brief 2D Texture data.
///
class Texture2D
{
public:
    ///
    /// \brief loadTexture Loads and caches texture data from image file.
    ///
    /// Texture2D object will automatically clean up cache and GPU data on
    /// destruction. Do NOT call glDeleteTextures on this texture's id.
    ///
    /// \param imageFilepath Filepath to the image.
    /// \exception age::LoadError Failed to load image data from file.
    ///
    explicit Texture2D(const std::string &imageFilepath);
    
    ///
    /// \brief Creates a 2D texture of a solid color.
    ///
    /// \param color RGB values between 0.0 and 1.0
    ///
    explicit Texture2D(const glm::vec3 &color);
    
    ///
    /// \brief bind Binds this texture to the GPU for rendering.
    ///
    void bind();

private:
    std::shared_ptr<unsigned int> id;
};

} // namespace age