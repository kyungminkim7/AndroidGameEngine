#pragma once

#include "GameObject.h"

#include <vector>

#include <glm/vec3.hpp>

namespace age {

///
/// \brief 3D box object.
///
class Box : public GameObject {
public:
    explicit Box(const std::vector<std::string> &diffuseTextureFilepaths={},
                 const std::vector<std::string> &specularTextureFilepaths={});
};

} // namespace age