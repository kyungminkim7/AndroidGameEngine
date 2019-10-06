#pragma once

#include "GameObject.h"

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Texture2D.h"

namespace age {

class ARPlane : public GameObject {
public:
    explicit ARPlane(const Texture2D &texture);
    ~ARPlane();

    void render(ShaderProgram *shader) override;

    void setDimensions(const glm::vec2 &dimensions);
    void setCollisionDiameter(float diameter);

private:
    std::vector<glm::vec2> generateTextureCoordinates(float scale);

    unsigned int vao;
    unsigned int vbo;
    unsigned int ebo;
    Texture2D texture;

    unsigned int numVertices;
    unsigned int numIndices;

    unsigned int textureCoordinatesOffset;

    bool visible = true;
};

} // namespace age