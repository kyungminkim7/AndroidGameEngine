#include <android_game_engine/Quad.h>

#include <algorithm>
#include <memory>

#include <BulletCollision/CollisionShapes/btBox2dShape.h>
#include <LinearMath/btVector3.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/Mesh.h>
#include <android_game_engine/VertexArray.h>

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

const std::vector<glm::vec2> textureCoordinates {
    {1.0f, 0.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 1.0f}
};

const std::vector<glm::uvec3> indices {
    {0u, 1u, 2u},
    {2u, 3u, 0u}
};

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
    std::vector<glm::vec2> repeatTextureCoordinates(textureCoordinates);
    std::transform(repeatTextureCoordinates.begin(), repeatTextureCoordinates.end(),
                   repeatTextureCoordinates.begin(),
                   [numTextureRepeat](const auto &tc){
                       return glm::vec2(tc.x * numTextureRepeat.x, tc.y * numTextureRepeat.y);
                   });
    auto vao = std::make_shared<VertexArray>(positions, normals, repeatTextureCoordinates, indices);
    std::shared_ptr<Meshes> meshes(new Meshes{Mesh(std::move(vao),
                                                   diffuseTextures,
                                                   specularTextures)});
    this->setMesh(std::move(meshes));
    
    // Create collision shape
    this->setCollisionShape(std::make_unique<btBox2dShape>(btVector3(0.5f, 0.5f, 0.0f)));
    this->setUnscaledDimensions({1.0f, 1.0f, 0.0f});
}

} // namespace age