#include <android_game_engine/Box.h>

#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/Mesh.h>
#include <android_game_engine/VertexBufferObject.h>

namespace {

const std::vector<glm::vec3> positions {
    // back face
    {-0.5f, -0.5f, -0.5f}, // bottom-left
    {0.5f,  0.5f, -0.5f}, // top-right
    {0.5f, -0.5f, -0.5f}, // bottom-right
    {-0.5f,  0.5f, -0.5f}, // top-left
    // front face
    {-0.5f, -0.5f,  0.5f}, // bottom-left
    {0.5f, -0.5f,  0.5f}, // bottom-right
    {0.5f,  0.5f,  0.5f}, // top-right
    {-0.5f,  0.5f,  0.5f}, // top-left
    // left face
    {-0.5f,  0.5f,  0.5f}, // top-right
    {-0.5f,  0.5f, -0.5f}, // top-left
    {-0.5f, -0.5f, -0.5f}, // bottom-left
    {-0.5f, -0.5f,  0.5f}, // bottom-right
    // right face
    {0.5f,  0.5f,  0.5f}, // top-left
    {0.5f, -0.5f, -0.5f}, // bottom-right
    {0.5f,  0.5f, -0.5f}, // top-right
    {0.5f, -0.5f,  0.5f}, // bottom-left
    // bottom face
    {-0.5f, -0.5f, -0.5f}, // top-right
    {0.5f, -0.5f, -0.5f}, // top-left
    {0.5f, -0.5f,  0.5f}, // bottom-left
    {-0.5f, -0.5f,  0.5f}, // bottom-right
    // top face
    {-0.5f,  0.5f, -0.5f},  // top-left
    {0.5f,  0.5f , 0.5f},  // bottom-right
    {0.5f,  0.5f, -0.5f},  // top-right
    {-0.5f,  0.5f,  0.5f}  // bottom-left
};

const std::vector<glm::vec3> normals {
    // back face
    {0.0f,  0.0f, -1.0f}, // bottom-left
    {0.0f,  0.0f, -1.0f}, // top-right
    {0.0f,  0.0f, -1.0f}, // bottom-right
    {0.0f,  0.0f, -1.0f}, // top-left
    // front face
    {0.0f,  0.0f,  1.0f}, // bottom-left
    {0.0f,  0.0f,  1.0f}, // bottom-right
    {0.0f,  0.0f,  1.0f}, // top-right
    {0.0f,  0.0f,  1.0f}, // top-left
    // left face
    {-1.0f,  0.0f,  0.0f}, // top-right
    {-1.0f,  0.0f,  0.0f}, // top-left
    {-1.0f,  0.0f,  0.0f}, // bottom-left
    {-1.0f,  0.0f,  0.0f}, // bottom-right
    // right face
    {1.0f,  0.0f,  0.0f}, // top-left
    {1.0f,  0.0f,  0.0f}, // bottom-right
    {1.0f,  0.0f,  0.0f}, // top-right
    {1.0f,  0.0f,  0.0f}, // bottom-left
    // bottom face
    {0.0f, -1.0f,  0.0f}, // top-right
    {0.0f, -1.0f,  0.0f}, // top-left
    {0.0f, -1.0f,  0.0f}, // bottom-left
    {0.0f, -1.0f,  0.0f}, // bottom-right
    // top face
    {0.0f,  1.0f,  0.0f}, // top-left
    {0.0f,  1.0f,  0.0f}, // bottom-right
    {0.0f,  1.0f,  0.0f}, // top-right
    {0.0f,  1.0f,  0.0f}  // bottom-left
};

const std::vector<glm::vec2> textureCoords {
    // back face
    {0.0f, 0.0f}, // bottom-left
    {1.0f, 1.0f}, // top-right
    {1.0f, 0.0f}, // bottom-right
    {0.0f, 1.0f}, // top-left
    // front face
    {0.0f, 0.0f}, // bottom-left
    {1.0f, 0.0f}, // bottom-right
    {1.0f, 1.0f}, // top-right
    {0.0f, 1.0f}, // top-left
    // left face
    {1.0f, 0.0f}, // top-right
    {1.0f, 1.0f}, // top-left
    {0.0f, 1.0f}, // bottom-left
    {0.0f, 0.0f}, // bottom-right
    // right face
    {1.0f, 0.0f}, // top-left
    {0.0f, 1.0f}, // bottom-right
    {1.0f, 1.0f}, // top-right
    {0.0f, 0.0f}, // bottom-left
    // bottom face
    {0.0f, 1.0f}, // top-right
    {1.0f, 1.0f}, // top-left
    {1.0f, 0.0f}, // bottom-left
    {0.0f, 0.0f}, // bottom-right
    // top face
    {0.0f, 1.0f}, // top-left
    {1.0f, 0.0f}, // bottom-right
    {1.0f, 1.0f}, // top-right
    {0.0f, 0.0f}  // bottom-left
};

const std::vector<glm::uvec3> indices {
    // back face
    {0u, 1u, 2u},
    {1u, 0u, 3u},
    // front face
    {4u, 5u, 6u},
    {6u, 7u, 4u},
    // left face
    {8u, 9u, 10u},
    {10u, 11u, 8u},
    // right face
    {12u, 13u, 14u},
    {13u, 12u, 15u},
    // bottom face
    {16u, 17u, 18u},
    {18u, 19u, 16u},
    // top face
    {20u, 21u, 22u},
    {21u, 20u, 23u}
};

std::weak_ptr<age::VertexBufferObject> vboCache;
std::weak_ptr<age::ElementBufferObject> eboCache;

} // namespace

namespace age {

Box::Box(const std::vector<std::string> &diffuseTextureFilepaths,
         const std::vector<std::string> &specularTextureFilepaths) {
    auto vbo = vboCache.lock();
    if (!vbo) {
        vbo = std::make_shared<VertexBufferObject>(positions, normals, textureCoords);
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