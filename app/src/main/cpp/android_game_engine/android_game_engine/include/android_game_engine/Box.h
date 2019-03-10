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
    explicit Box(const std::vector<std::string> &textureFilepaths={},
                 const glm::vec3 &dimensions=glm::vec3(1.0f));
};

} // namespace age