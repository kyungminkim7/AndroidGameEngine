#pragma once

#include "GameObject.h"

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Texture2D.h"

namespace age {

class ARPlaneCircle : public GameObject {
public:
    explicit ARPlaneCircle(const Texture2D &texture,
                           unsigned int numVertices=25);
    ~ARPlaneCircle();

    void render(ShaderProgram *shader) override;

    void setDimensions(const glm::vec2 &dimensions);
    void setColor(const glm::vec3 &color);

private:
    std::vector<glm::vec2> generateTextureCoordinates(float scale);

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    Texture2D texture;

    unsigned int numIndices;

    unsigned int numVertices;
    glm::vec3 color;

    unsigned int textureCoordinatesOffset;
};

} // namespace age