#pragma once

#include "GameObject.h"

#include <set>

#include <glm/vec3.hpp>

namespace age {

///
/// \brief 3D box object.
///
class Box : public GameObject {
public:
    explicit Box(const std::set<std::string> &diffuseTextureFilepaths={},
                 const std::set<std::string> &specularTextureFilepaths={});
};

} // namespace age