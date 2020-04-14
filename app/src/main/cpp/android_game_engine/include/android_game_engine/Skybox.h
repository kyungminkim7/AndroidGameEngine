#pragma once

#include <string>

namespace age {

class ShaderProgram;

///
/// \brief The Skybox class represents a skybox loaded from a cubemap.
///
class Skybox {
public:
    ///
    /// \brief Skybox Creates a cubemap for a skybox.
    /// \param imageFilepaths 6 images for each side of the skybox in the order of:
    ///                       front, back, right, left, top, bottom
    /// \exception age::LoadError Failed to load texture data from image file.
    ///
    explicit Skybox(const std::array<std::string, 6> &imageFilepaths);
    ~Skybox();

    Skybox(Skybox &&) noexcept = default;
    Skybox& operator=(Skybox &&) noexcept = default;
    
    void render(ShaderProgram *shader);

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture;
};

} // namespace age