#include <android_game_engine/Quad.h>

#include <algorithm>
#include <memory>

#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <LinearMath/btVector3.h>
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

Quad::Quad(const std::set<std::string> &diffuseTextureFilepaths,
           const std::set<std::string> &specularTextureFilepaths,
           const glm::vec2 &numTextureRepeat)
        : GameObject() {
    std::vector<Texture2D> diffuseTextures, specularTextures;
    
    for (const auto& filepath : diffuseTextureFilepaths) {
        diffuseTextures.emplace_back(filepath);
    }
    
    for (const auto& filepath : specularTextureFilepaths) {
        specularTextures.emplace_back(filepath);
    }
    
    this->init(diffuseTextures, specularTextures, numTextureRepeat);
}

Quad::Quad(const std::vector<age::Texture2D> &diffuseTextures,
           const std::vector<age::Texture2D> &specularTextures,
           const glm::vec2 &numTextureRepeat){
    this->init(diffuseTextures, specularTextures, numTextureRepeat);
}
           
void Quad::init(const std::vector<age::Texture2D> &diffuseTextures,
                const std::vector<age::Texture2D> &specularTextures,
                const glm::vec2 &numTextureRepeat) {
    // Create mesh
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
        eboCache = ebo;
    }
    
    std::shared_ptr<Meshes> meshes(new Meshes{Mesh(std::move(vbo), std::move(ebo),
                                                   diffuseTextures,
                                                   specularTextures)});
    this->setMesh(std::move(meshes));
    
    // Create collision shape
    this->setCollisionShape(std::make_unique<btBox2dShape>(btVector3(0.5f, 0.5f, 0.0f)));
    this->setUnscaledDimensions({1.0f, 1.0f, 0.0f});
}

} // namespace age