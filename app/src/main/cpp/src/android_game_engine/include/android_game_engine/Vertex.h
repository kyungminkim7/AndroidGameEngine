#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace age {

struct Vertex {
    Vertex(glm::vec3 &&position,
           glm::vec3 &&normal,
           glm::vec2 &&textureCoordinates);

    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

} // namespace age