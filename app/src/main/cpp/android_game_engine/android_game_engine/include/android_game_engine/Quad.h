#pragma once

#include <set>

#include "GameObject.h"

namespace age {

///
/// \brief The Quad class is a Game Object that is a flat square.
///
class Quad : public GameObject {
public:
    ///
    /// \brief Quad Creates a quad with the specified number of times to repeate the texture.
    /// \param diffuseTextureFilepaths
    /// \param specularTextureFilepaths
    /// \param numTextureRepeat Number of times to repeat the texture in the x and y directions.
    ///
    explicit Quad(const std::set<std::string> &diffuseTextureFilepaths={},
                  const std::set<std::string> &specularTextureFilepaths={},
                  const glm::vec2 &numTextureRepeat = glm::vec2(1.0f));

private:
    std::string imageFilepath;
};

} // namespace age