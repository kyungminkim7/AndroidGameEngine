#include <android_game_engine/Quad.h>

#include <algorithm>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/Mesh.h>
#include <android_game_engine/VertexBufferObject.h>

namespace {

const std::vector<glm::vec3> positions {
    {0.5f, -0.5f,  0.0f},
    {0.5f,  0.5f,  0.0f},
    {-0.5f,  0.5f,  0.0f},
    {-0.5f, -0.5f,  0.0f}
};

const std::vector<glm::vec3> normals {
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f}
};

const std::vector<glm::vec2> textureCoords {
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f}
};

const std::vector<glm::uvec3> indices {
    {0, 1, 2},
    {2, 3, 0}
};

std::weak_ptr<age::ElementBufferObject> eboCache;

} // namespace

namespace age {

Quad::Quad(const std::vector<std::string> &diffuseTextureFilepaths,
           const std::vector<std::string> &specularTextureFilepaths,
           const glm::vec2 &numTextureRepeat)
        : GameObject() {
    std::vector<glm::vec2> repeatTextureCoords(textureCoords);
    std::transform(repeatTextureCoords.begin(), repeatTextureCoords.end(),
                   repeatTextureCoords.begin(),
                   [numTextureRepeat](const auto &tc){
                       return glm::vec2(tc.x * numTextureRepeat.x, tc.y * numTextureRepeat.y);
                   });
    auto vbo = std::make_shared<VertexBufferObject>(positions, normals, repeatTextureCoords);
    
    auto ebo = eboCache.lock();
    if (!ebo) {
        ebo = std::make_shared<ElementBufferObject>(indices);
    }
    
    std::shared_ptr<Meshes> meshes(new Meshes{Mesh(std::move(vbo), std::move(ebo),
                                                   diffuseTextureFilepaths,
                                                   specularTextureFilepaths)});
    this->setMesh(std::move(meshes));
}

} // namespace age