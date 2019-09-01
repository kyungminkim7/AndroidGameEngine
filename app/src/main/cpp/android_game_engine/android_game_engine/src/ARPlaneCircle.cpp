#include <android_game_engine/ARPlaneCircle.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <glm/mat2x2.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <android_game_engine/ShaderProgram.h>

namespace {

constexpr auto positionStride = sizeof(glm::vec3);
constexpr auto textureCoordinatesStride = sizeof(glm::vec2);
constexpr auto opacityStride = sizeof(float);

} // namespace

namespace age {

ARPlaneCircle::ARPlaneCircle(const Texture2D &texture,
                             unsigned int numVertices)
                             : GameObject(),
                             texture(texture) ,numVertices(numVertices), color(glm::vec3(1.0f)) {
    // Generate vertex data
    std::vector<glm::vec3> positions;
    std::vector<float> opacities(this->numVertices + 1, 1.0f);
    std::vector<glm::uvec3> indices;

    positions.reserve(this->numVertices + 1);
    indices.reserve(this->numVertices);

    // 1st vertex is the center
    positions.push_back(glm::vec3(0.0f));

    for (auto i = 1u; i < this->numVertices + 1; ++i) {
        auto angle = glm::radians(360.0f / this->numVertices) * (i - 1);
        positions.emplace_back(glm::rotateZ(glm::vec3(0.5f, 0.0f, 0.0f), angle));

        auto lastIndex = (i + 1) % (numVertices + 1);
        indices.emplace_back(glm::uvec3(0u, i, lastIndex ? lastIndex : 1u));
    }

    auto textureCoordinates = this->generateTextureCoordinates(1.0f);

    // Load vertex data onto GPU
    const auto positionsSize_bytes = positions.size() * positionStride;
    const auto textureCoordinatesSize_bytes = textureCoordinates.size() * textureCoordinatesStride;
    const auto opacitiesSize_bytes = opacityStride;

    this->textureCoordinatesOffset = positionsSize_bytes;
    const auto opacitiesOffset = this->textureCoordinatesOffset + textureCoordinatesSize_bytes;

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    // Store vertex data
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, positionsSize_bytes + textureCoordinatesSize_bytes + opacitiesSize_bytes,
                 nullptr, GL_DYNAMIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, positionsSize_bytes, positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, this->textureCoordinatesOffset, textureCoordinatesSize_bytes,
                    textureCoordinates.data());
    glBufferSubData(GL_ARRAY_BUFFER, opacitiesOffset, opacitiesSize_bytes, opacities.data());

    // Assign vertex attributes
    glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, positionStride,
                          reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0u);

    glVertexAttribPointer(1u, 2, GL_FLOAT, GL_FALSE, textureCoordinatesStride,
                          reinterpret_cast<GLvoid*>(this->textureCoordinatesOffset));
    glEnableVertexAttribArray(1u);

    glVertexAttribPointer(2u, 1, GL_FLOAT, GL_FALSE, opacityStride,
                          reinterpret_cast<GLvoid*>(opacitiesOffset));
    glEnableVertexAttribArray(2u);

    // Store indices data
    this->numIndices = indices.size() * 3;

    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3),
                 indices.data(), GL_STATIC_DRAW);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create collision shape
    const auto thickness = 0.01f;
    this->setCollisionShape(std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, thickness)));
    this->setUnscaledDimensions({1.0f, 1.0f, thickness});
}

std::vector<glm::vec2> ARPlaneCircle::generateTextureCoordinates(float scale) {
    std::vector<glm::vec2> textureCoordinates;
    textureCoordinates.reserve(this->numVertices + 1);

    textureCoordinates.push_back(glm::vec2(0.5f));
    for (auto i = 1u; i < this->numVertices + 1; ++i) {
        auto angle = glm::radians(360.0f / this->numVertices) * (i - 1);
        textureCoordinates.emplace_back((glm::vec2(0.5f) +
                                         glm::mat2({0.0f, -1.0f}, {-1.0f, 0.0f}) * glm::rotate(glm::vec2(0.5f * scale, 0.0f), angle)));
    }

    return textureCoordinates;
}

ARPlaneCircle::~ARPlaneCircle() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
    glDeleteBuffers(1, &this->ebo);
}

void ARPlaneCircle::render(age::ShaderProgram *shader) {
    shader->setUniform("model", this->getModelMatrix());

    shader->setUniform("color", this->color);
    glActiveTexture(GL_TEXTURE0);
    shader->setUniform("planeTexture", 0);
    this->texture.bind();

    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, this->numIndices,
                   GL_UNSIGNED_INT, reinterpret_cast<const GLvoid*>(0));
}

void ARPlaneCircle::setDiameter(float diameter){
    this->setScale({diameter, diameter, 1.0f});

    // Update vertex texture coordinates
    auto textureCoordinates = this->generateTextureCoordinates(diameter);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferSubData(GL_ARRAY_BUFFER, this->textureCoordinatesOffset,
                    textureCoordinates.size() * textureCoordinatesStride,
                    textureCoordinates.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ARPlaneCircle::setColor(const glm::vec3 &color) {this->color = color;}

} // namespace age