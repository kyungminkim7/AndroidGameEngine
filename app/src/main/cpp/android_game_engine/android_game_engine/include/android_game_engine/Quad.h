#pragma once

#include "GameObject.h"

#include <set>
#include <vector>

namespace age {

///
/// \brief The Quad class is a Game Object that is a flat square.
///
class Quad : public GameObject {
public:
    ///
    /// \brief Quad Creates a quad with the specified number of times to repeat the texture.
    /// \param diffuseTextureFilepaths
    /// \param specularTextureFilepaths
    /// \param numTextureRepeat Number of times to repeat the texture in the x and y directions.
    ///
    explicit Quad(const std::set<std::string> &diffuseTextureFilepaths={},
                  const std::set<std::string> &specularTextureFilepaths={},
                  const glm::vec2 &numTextureRepeat = glm::vec2(1.0f));
    
    ///
    /// \brief Quad Creates a quad with the specified number of times to repeat the texture.
    /// \param diffuseTextures
    /// \param specularTextures
    /// \param numTextureRepeat Number of times to repeat the texture in the x and y directions.
    ///
    explicit Quad(const std::vector<Texture2D> &diffuseTextures={},
                  const std::vector<Texture2D> &specularTextures={},
                  const glm::vec2 &numTextureRepeat = glm::vec2(1.0f));
    
private:
    void init(const std::vector<Texture2D> &diffuseTextures,
              const std::vector<Texture2D> &specularTextures,
              const glm::vec2 &numTextureRepeat);
};

} // namespace age