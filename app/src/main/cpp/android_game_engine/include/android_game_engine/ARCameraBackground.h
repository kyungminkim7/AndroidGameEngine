#pragma once

#include <vector>

#include <arcore_c_api.h>
#include <glm/vec2.hpp>

namespace age {

class ShaderProgram;

class ARCameraBackground {
public:
    ARCameraBackground();
    ~ARCameraBackground();

    ARCameraBackground(ARCameraBackground &&) = default;
    ARCameraBackground& operator=(ARCameraBackground &&) = default;

    void onUpdate(const ArSession *arSession, const ArFrame *arFrame);
    void render(ShaderProgram *shader, int64_t arFrameTimestamp);

    unsigned int getTexture() const;

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture;

    std::vector<glm::vec2> textureCoordinates;
    bool textureCoordinatesInitialized = false;
};

} // namespace age