#pragma once

#include <glm/vec2.hpp>

namespace age {

struct TouchEvent {
    int id;
    glm::vec2 coordinate;

    TouchEvent(int id, const glm::vec2 &coordinate) : id(id), coordinate(coordinate) {}
};

} // namespace age