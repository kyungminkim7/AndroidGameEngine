#include <android_game_engine/Quad.h>

#include <memory>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/Mesh.h>
#include <android_game_engine/VertexBufferObject.h>

namespace {

const std::vector<float> positions {
        0.5f, -0.5f,  0.0f,
        0.5f,  0.5f,  0.0f,
        -0.5f,  0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
};

const std::vector<float> normals {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
};

const std::vector<float> textureCoords {
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f
};

const std::vector<unsigned int> indices {
        0, 1, 2,
        2, 3, 0
};

std::weak_ptr<age::VertexBufferObject> vboCache;
std::weak_ptr<age::ElementBufferObject> eboCache;

} // namespace

namespace age {

Quad::Quad(const std::vector<std::string> &diffuseTextureFilepaths,
           const std::vector<std::string> &specularTextureFilepaths,
           const glm::vec2 &numTextureRepeat)
        : GameObject() {
    auto vbo = vboCache.lock();
    if (!vbo) {
        std::vector<float> repeatTextureCoords(textureCoords);
        for (auto i = 0u; i < repeatTextureCoords.size(); ++i) {
            repeatTextureCoords[i] *= (i % 2 == 0 ? numTextureRepeat.x : numTextureRepeat.y);
        }
        
        vbo = std::make_shared<VertexBufferObject>(positions, normals, repeatTextureCoords);
    }
    
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