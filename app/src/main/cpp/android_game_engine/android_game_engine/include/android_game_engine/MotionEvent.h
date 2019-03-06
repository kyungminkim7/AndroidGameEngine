#pragma once

#include <unordered_map>

#include <glm/vec2.hpp>

namespace age {

using MotionEvent = std::unordered_map<int, glm::vec2>;

} // namespace age