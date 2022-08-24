#pragma once

#include "Light.h"

#include <chrono>

namespace age {

///
/// \brief The LightDirectional class represents a directional light with uniform
/// orthonormal projection.
///
class LightDirectional : public Light {
public:
    LightDirectional(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                     float left, float right, float bottom, float top, float nearPlane, float farPlane);
    
    ///
    /// \brief getProjectionMatrix Returns the orthonormal projection matrix of the light.
    /// \return The orthonormal projection matrix of the light.
    ///
    glm::mat4 getProjectionMatrix() const override;
    
    void render(ShaderProgram *shader) override;

private:
    float left;
    float right;
    float bottom;
    float top;
    float nearPlane;
    float farPlane;
};

} // namespace age