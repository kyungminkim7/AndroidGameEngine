#include <android_game_engine/ARCameraBackground.h>

#include <GLES3/gl32.h>
#include <GLES2/gl2ext.h>

#include <android_game_engine/ShaderProgram.h>

namespace {

const std::vector<glm::vec2> positions {
    {-1.0f, -1.0f},
    { 1.0f, -1.0f},
    {-1.0f,  1.0f},
    { 1.0f,  1.0f}
};

constexpr auto positionsStride = sizeof(glm::vec2);
constexpr auto textureCoordinatesStride = sizeof(glm::vec2);

constexpr auto positionsSize_bytes = 4 * positionsStride;
constexpr auto textureCoordinatesSize_bytes = 4 * textureCoordinatesStride;

} // namespace

namespace age {

ARCameraBackground::ARCameraBackground() : textureCoordinates(positions.size()) {
    // Store vertex data
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    glBufferData(GL_ARRAY_BUFFER,
                 positionsSize_bytes + textureCoordinatesSize_bytes,
                 nullptr, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, positionsSize_bytes, positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, positionsSize_bytes,
                    textureCoordinatesSize_bytes, this->textureCoordinates.data());

    // Assign vertex attributes
    glVertexAttribPointer(0u, 2, GL_FLOAT, GL_FALSE, positionsStride, reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0u);

    glVertexAttribPointer(1u, 2, GL_FLOAT, GL_FALSE, textureCoordinatesStride,
                          reinterpret_cast<GLvoid*>(positionsSize_bytes));
    glEnableVertexAttribArray(1u);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Store texture
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, this->texture);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, 0);
}

ARCameraBackground::~ARCameraBackground() {
    glDeleteTextures(1, &this->texture);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void ARCameraBackground::onUpdate(const ArSession *arSession, const ArFrame *arFrame) {
    int32_t geometryChanged = 0;
    ArFrame_getDisplayGeometryChanged(arSession, arFrame, &geometryChanged);
    if (!geometryChanged || !this->textureCoordinatesInitialized) {
        ArFrame_transformCoordinates2d(
                arSession, arFrame, AR_COORDINATES_2D_OPENGL_NORMALIZED_DEVICE_COORDINATES,
                positions.size(), reinterpret_cast<const float*>(positions.data()),
                AR_COORDINATES_2D_TEXTURE_NORMALIZED,
                reinterpret_cast<float*>(this->textureCoordinates.data()));
        this->textureCoordinatesInitialized = true;

        // Store new texture coordinates
        glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
        glBufferSubData(GL_ARRAY_BUFFER, positionsSize_bytes,
                        textureCoordinatesSize_bytes, this->textureCoordinates.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void ARCameraBackground::render(age::ShaderProgram *shader, int64_t arFrameTimestamp) {
    if (arFrameTimestamp == 0) return;

    glDepthMask(GL_FALSE);

    glBindVertexArray(this->vao);

    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("backgroundTexture", 0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, this->texture);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDepthMask(GL_TRUE);
}

unsigned int ARCameraBackground::getTexture() const {return this->texture;}

} // namespace age