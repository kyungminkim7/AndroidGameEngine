#include <android_game_engine/Light.h>

#include <android_game_engine/ShaderProgram.h>

namespace age {

Light::Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}

} // namespace age