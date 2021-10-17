#include <android_game_engine/GameObject.h>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <memory>
#include <numeric>
#include <tuple>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/AssimpIOSystem.h>
#include <android_game_engine/Exception.h>
#include <android_game_engine/ShaderProgram.h>
#include <android_game_engine/Vertex.h>
#include <android_game_engine/VertexArray.h>

namespace {

unsigned int getNumMeshes(const aiNode *node);
age::Mesh processMesh(const aiMesh *mesh, const aiScene *scene, const std::string &dir);
std::vector<std::string> loadMaterialTextures(const aiMaterial *material, aiTextureType type);

unsigned int getNumMeshes(const aiNode *node) {
    return node->mNumMeshes + std::accumulate(node->mChildren, node->mChildren + node->mNumChildren, 0u,
                                              [](const auto sum, const auto child){ return sum + getNumMeshes(child); });
}

age::Mesh processMesh(const aiMesh *mesh, const aiScene *scene, const std::string &dir) {
    // Copy vertex data
    std::vector<age::Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);
    for (auto i = 0u; i < mesh->mNumVertices; ++i) {
        const auto &vertex = mesh->mVertices[i];
        const auto &normal = mesh->mNormals[i];
        vertices.emplace_back(glm::vec3(vertex.x, vertex.y, vertex.z),
                              glm::vec3(normal.x, normal.y, normal.z),
                              mesh->mTextureCoords[0] ?
                                glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) :
                                glm::vec2(0.0f));
    }

    // Copy index data
    std::vector<unsigned int> indices;
    const auto numIndices = std::accumulate(mesh->mFaces, mesh->mFaces + mesh->mNumFaces, 0u,
                                            [](const auto sum, const auto &face){ return sum + face.mNumIndices; });
    indices.reserve(numIndices);
    std::for_each(mesh->mFaces, mesh->mFaces + mesh->mNumFaces,
                  [&indices](const auto &face){ indices.insert(indices.cend(),
                                                               face.mIndices,
                                                               face.mIndices + face.mNumIndices); });

    // Load textures
    std::vector<std::string> diffuseTextures, specularTextures;
    if (mesh->mMaterialIndex >= 0) {
        const auto material = scene->mMaterials[mesh->mMaterialIndex];
        diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE);
        specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR);

        const auto prependDir = [&dir](const std::string &filename){ return dir + "/" + filename; };
        std::transform(diffuseTextures.begin(), diffuseTextures.end(),
                       diffuseTextures.begin(), prependDir);
        std::transform(specularTextures.begin(), specularTextures.end(),
                       specularTextures.begin(), prependDir);
    }

    return age::Mesh(std::make_shared<age::VertexArray>(vertices, indices),
            diffuseTextures, specularTextures);
}

std::vector<std::string> loadMaterialTextures(const aiMaterial *material, aiTextureType type) {
    std::vector<std::string> textures;
    textures.reserve(material->GetTextureCount(type));
    for (auto i = 0u; i < material->GetTextureCount(type); ++i) {
        aiString filename;
        material->GetTexture(type, i, &filename);
        textures.emplace_back(filename.C_Str());
    }
    return textures;
}

template<typename Iter>
glm::vec3 getBound(Iter begin, Iter end) {
    const auto compareAbsX = [](const auto &v1, const auto &v2){ return std::abs(v1.x) < std::abs(v2.x); };
    const auto compareAbsY = [](const auto &v1, const auto &v2){ return std::abs(v1.y) < std::abs(v2.y); };
    const auto compareAbsZ = [](const auto &v1, const auto &v2){ return std::abs(v1.z) < std::abs(v2.z); };
    return {
        std::abs(std::max_element(begin, end, compareAbsX)->x),
        std::abs(std::max_element(begin, end, compareAbsY)->y),
        std::abs(std::max_element(begin, end, compareAbsZ)->z)
    };
}

glm::vec3 getHalfExtents(const aiNode *node, const aiScene *scene) {
    std::vector<glm::vec3> bounds;
    bounds.reserve(node->mNumMeshes + node->mNumChildren);

    std::transform(node->mMeshes, node->mMeshes + node->mNumMeshes,
                   std::back_inserter(bounds),
                   [scene](const auto i){
                        const auto mesh = scene->mMeshes[i];
                        return getBound(mesh->mVertices, mesh->mVertices + mesh->mNumVertices);
                   });

    std::transform(node->mChildren, node->mChildren + node->mNumChildren,
                   std::back_inserter(bounds),
                   [scene](const auto child){ return getHalfExtents(child, scene); });

    return getBound(bounds.cbegin(), bounds.cend());
}

} // namespace

namespace age {

GameObject::GameObject() : meshes(std::make_shared<Meshes>()) {}

GameObject::GameObject(const std::string &modelFilepath) : meshes(std::make_shared<Meshes>()) {
    Assimp::Importer importer;
    importer.SetIOHandler(new AssimpIOSystem);

    const auto scene = importer.ReadFile(modelFilepath, aiProcess_Triangulate);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw LoadError("Failed to load model from: " + modelFilepath);
    }

    // Load meshes
    this->meshes->reserve(getNumMeshes(scene->mRootNode));
    const auto dir = modelFilepath.substr(0, modelFilepath.find_last_of("/\\"));
    this->processNode(scene->mRootNode, scene, dir);

    // Create collision box
    const auto halfExtents = getHalfExtents(scene->mRootNode, scene);
    this->unscaledDimensions = halfExtents * 2.0f;
    this->setCollisionShape(std::make_unique<btBoxShape>(btVector3{halfExtents.x,
                                                                   halfExtents.y,
                                                                   halfExtents.z}));
}

void GameObject::processNode(const aiNode *node, const aiScene *scene, const std::string &dir) {
    std::transform(node->mMeshes, node->mMeshes + node->mNumMeshes,
                   std::back_inserter(*this->meshes),
                   [scene, &dir](const auto i){ return processMesh(scene->mMeshes[i], scene, dir); });
    std::for_each(node->mChildren, node->mChildren + node->mNumChildren,
                  [this, scene, &dir](const auto child){ this->processNode(child, scene, dir); });
}

void GameObject::onUpdate(std::chrono::duration<float> updateDuration) {}

void GameObject::updateFromPhysics() {
    if (this->physicsBody && this->physicsBody->isActive()) {
        auto transform = this->physicsBody->getTransform();
        this->model.setOrientation(std::get<0>(transform));
        this->model.setPosition(std::get<1>(transform));
    }
}

void GameObject::renderShadow(ShaderProgram *shader) {
    shader->setUniform("model", this->model.getModelMatrix());

    std::for_each(this->meshes->begin(), this->meshes->end(),
                  [shader](auto &mesh){ mesh.renderVAO(shader); });
}

void GameObject::render(ShaderProgram *shader) {
    shader->setUniform("model", this->model.getModelMatrix());
    shader->setUniform("normal", this->model.getNormalMatrix());

    shader->setUniform("material.specularExponent", this->specularExponent);

    std::for_each(this->meshes->begin(), this->meshes->end(),
                  [shader](auto &mesh){
                      mesh.bindTextures(shader);
                      mesh.renderVAO(shader);
                  });
}

void GameObject::setMesh(std::shared_ptr<Meshes> mesh) {
    this->meshes = std::move(mesh);
}

void GameObject::setPosition(const glm::vec3 &position) {
    this->model.setPosition(position);
    
    if (this->physicsBody) {
        this->physicsBody->setPosition(position);
    }
}

void GameObject::setOrientation(const glm::mat3& orientation) {
    this->model.setOrientation(orientation);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(orientation);
    }
}

void GameObject::setOrientation(const glm::vec3 &orientationX,
                                const glm::vec3 &orientationY,
                                const glm::vec3 &orientationZ) {
    this->model.setOrientation(orientationX, orientationY, orientationZ);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::setLookAtPoint(const glm::vec3 &lookAtPoint) {
    this->model.setLookAtPoint(lookAtPoint);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::setLookAtDirection(const glm::vec3 &lookAtDirection) {
    this->model.setLookAtDirection(lookAtDirection);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::setNormalDirection(const glm::vec3 &normalDirection) {
    this->model.setNormalDirection(normalDirection);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::rotate(float angle_rad, const glm::vec3 &axis) {
    this->model.rotate(angle_rad, axis);
    
    if (this->physicsBody) {
        this->physicsBody->setOrientation(this->model.getOrientation());
    }
}

void GameObject::translate(const glm::vec3 &translation) {
    this->model.translate(translation);
    
    if (this->physicsBody) {
        this->physicsBody->setPosition(this->model.getPosition());
    }
}

void GameObject::translateInLocalFrame(const glm::vec3 &translation) {
    this->model.translateInLocalFrame(translation);
    
    if (this->physicsBody) {
        this->physicsBody->setPosition(this->model.getPosition());
    }
}

void GameObject::setScale(const glm::vec3& scale) {
    this->model.setScale(scale);
    
    if (this->physicsBody) {
        this->physicsBody->setScale(scale);
    }
}

void GameObject::setCollisionShapeScale(const glm::vec3 &scale) {
    this->physicsBody->setScale(scale);
}

void GameObject::setDamping(float linearDamping, float angularDamping) {
    this->physicsBody->setDamping(linearDamping, angularDamping);
}

void GameObject::setSpecularExponent(float specularExponent) {
    this->specularExponent = specularExponent;
}

PhysicsRigidBody* GameObject::getPhysicsBody() {
    return this->physicsBody.get();
}

void GameObject::setMass(float mass) {
    if (this->physicsBody) {
        this->physicsBody->setMass(mass);
    }
}

void GameObject::setFriction(float friction) {
    this->physicsBody->setFriction(friction);
}

void GameObject::setCollisionShape(std::unique_ptr<btCollisionShape> collisionShape) {
    this->physicsBody = std::make_unique<PhysicsRigidBody>(this, std::move(collisionShape));
}

void GameObject::setUnscaledDimensions(const glm::vec3 &dimensions) {
    this->unscaledDimensions = dimensions;
}

} // namespace age
