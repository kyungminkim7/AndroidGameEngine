#pragma once

#include <cstdint>

namespace age {

class ShaderProgram;

class ImageMsgDisplay {
public:
    ImageMsgDisplay();
    ~ImageMsgDisplay();

    void bufferImage(const uint8_t imgMsgBuffer[]);
    void render(ShaderProgram *shader);

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture;

    unsigned int width = 0u;
    unsigned int height = 0u;
};

} // namespace age