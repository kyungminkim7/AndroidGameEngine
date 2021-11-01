#include "ImageMsgDisplay.h"

#include <vector>

#include <GLES3/gl32.h>
#include <glm/vec2.hpp>
#include <network/msgs/Image_generated.h>

#include <android_game_engine/ShaderProgram.h>

#include <android_game_engine/Log.h>

namespace {

constexpr auto positionsStride = sizeof(glm::vec2);
constexpr auto textureCoordinatesStride = sizeof(glm::vec2);

constexpr auto positionsSize_bytes = 4 * positionsStride;
constexpr auto textureCoordinatesSize_bytes = 4 * textureCoordinatesStride;

} // namespace

namespace age {

ImageMsgDisplay::ImageMsgDisplay() {
    const std::vector<glm::vec2> positions {
        {-1.0f, -1.0f},
        {1.0f, -1.0f},
        {-1.0f, 1.0f},
        {1.0f, 1.0}
    };

    const std::vector<glm::vec2> textureCoordinates {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f}
    };

    // Store vertex data
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glBufferData(GL_ARRAY_BUFFER,
            positionsSize_bytes + textureCoordinatesSize_bytes,
            nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, positionsSize_bytes, positions.data());
    glBufferSubData(GL_ARRAY_BUFFER,
                    positionsSize_bytes, textureCoordinatesSize_bytes,
                    textureCoordinates.data());

    // Assign vertex attributes
    glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, positionsStride, reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0u);

    glVertexAttribPointer(1u, 2, GL_FLOAT, GL_FALSE, textureCoordinatesStride,
                          reinterpret_cast<GLvoid*>(positionsSize_bytes));
    glEnableVertexAttribArray(1u);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Generate texture
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    const float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

ImageMsgDisplay::~ImageMsgDisplay() {
    glDeleteTextures(1, &this->texture);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void ImageMsgDisplay::bufferImage(const ntwk::Image &img) {
    GLenum format;
    switch (img.channels) {
        case 1:
            format = GL_R8;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            return;
    }

    // Update texture coordinates to properly scale to screen dimensions
    if (!(img.width == this->width && img.height == this->height)) {
        this->width = img.width;
        this->height = img.height;

        const std::vector<glm::vec2> textureCoordinates {
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f}
        };

        // Update texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferSubData(GL_ARRAY_BUFFER,
                positionsSize_bytes, textureCoordinatesSize_bytes,
                textureCoordinates.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Update texture size
        glBindTexture(GL_TEXTURE_2D, this->texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->width, this->height, 0,
                format, GL_UNSIGNED_BYTE, nullptr);
    }

    // Update texture
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height,
            format, GL_UNSIGNED_BYTE, img.data.get());
}

void ImageMsgDisplay::render(ShaderProgram *shader) {
    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("cameraImageTexture", 0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // Draw
    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

} // namespace age