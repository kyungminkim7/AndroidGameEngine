#pragma once

#include "GameObject.h"

#include <set>
#include <vector>

#include <glm/vec2.hpp>

#include "Texture2D.h"

namespace age {

///
/// \brief 3D box object.
///
class Box : public GameObject {
public:
    explicit Box(const std::set<std::string> &diffuseTextureFilepaths={},
                 const std::set<std::string> &specularTextureFilepaths={},
                 const glm::vec2 &numTextureRepeat = glm::vec2(1.0f));
    
    explicit Box(const std::vector<Texture2D> &diffuseTextures={},
                 const std::vector<Texture2D> &specularTextures={},
                 const glm::vec2 &numTextureRepeat = glm::vec2(1.0f));

private:
    void init(const std::vector<Texture2D> &diffuseTextures,
              const std::vector<Texture2D> &specularTextures,
              const glm::vec2 &numTextureRepeat);
};

} // namespace age