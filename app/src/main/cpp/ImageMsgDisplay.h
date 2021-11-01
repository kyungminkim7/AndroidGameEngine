#pragma once

#include <cstdint>

#include <network/Image.h>

namespace age {

class ShaderProgram;

class ImageMsgDisplay {
public:
    ImageMsgDisplay();
    ~ImageMsgDisplay();

    void bufferImage(const ntwk::Image &img);
    void render(ShaderProgram *shader);

private:
    unsigned int vao;
    unsigned int vbo;
    unsigned int texture;

    unsigned int width = 0u;
    unsigned int height = 0u;
};

} // namespace age