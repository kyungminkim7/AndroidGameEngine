#include <android_game_engine/GameObject.h>

#include <iostream>
#include <unordered_map>

//#include <assimp/Importer.hpp>
//#include <assimp/postprocess.h>
//#include <assimp/scene.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include <android_game_engine/Exception.h>
#include <android_game_engine/Mesh.h>
#include <android_game_engine/ShaderProgram.h>

namespace {

std::unordered_map<std::string, std::weak_ptr<age::GameObject::Meshes>> cachedMeshes;

///
/// \brief loadMeshes Loads and caches mesh data from model file.
///
/// Mesh caches will automatically be cleaned up as the last
/// reference to the returned shared_ptr<Meshes> is destroyed.
///
/// \param modelFilepath Absolute filepath to the model data.
/// \return Shared pointer to the model data (meshes).
/// \exception ge::LoadError Failed to load mesh data from model file.
/// \exception ge::LoadError Failed to load texture image from file.
///
//std::shared_ptr<Meshes> loadMeshes(const std::string &modelFilepath);
//void processNode(Meshes *meshes, const aiNode &node, const aiScene &scene, const std::string &modelDirectory);
//
//std::shared_ptr<Meshes> loadMeshes(const std::string &modelFilepath) {
//    auto modelFilenameIndex = modelFilepath.find_last_of('/');
//    const auto modelDirectory = modelFilepath.substr(0, modelFilenameIndex);
//    const auto modelFilename = modelFilepath.substr(modelFilenameIndex + 1);
//
//    // Check cached meshes to avoid reloading
//    auto meshes = cachedMeshes[modelFilename].lock();
//    if (meshes) return meshes;
//
//    // Load meshes from file
//    Assimp::Importer importer;
//    const auto scene = importer.ReadFile(modelFilepath,
//                                         aiProcess_Triangulate | aiProcess_FlipUVs);
//
//    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        throw ge::LoadError(importer.GetErrorString());
//    }
//
//    auto meshesDeleter = [modelFilename](auto meshes){
//        // Clear cache
//        cachedMeshes.erase(modelFilename);
//        delete meshes;
//    };
//    meshes = std::shared_ptr<Meshes>(new Meshes, meshesDeleter);
//
//    processNode(meshes.get(), *scene->mRootNode, *scene, modelDirectory);
//
//    std::cout << "Successfully loaded model from file: " << modelFilepath << "\n";
//    cachedMeshes[modelFilename] = meshes;
//    return meshes;
//}
//
//void processNode(Meshes *meshes, const aiNode &node, const aiScene &scene, const std::string &modelDirectory) {
//    // Process node's meshes.
//    for (unsigned int i = 0; i < node.mNumMeshes; ++i) {
//        const auto mesh = scene.mMeshes[node.mMeshes[i]];
//        const auto material = scene.mMaterials[mesh->mMaterialIndex];
//        meshes->push_back(std::make_unique<ge::Mesh>(*mesh, *material, modelDirectory));
//    }
//
//    // Recursively process children nodes.
//    for (unsigned int i = 0; i < node.mNumChildren; ++i) {
//        processNode(meshes, *node.mChildren[i], scene, modelDirectory);
//    }
//}

} // namespace

namespace age {

GameObject::GameObject() : meshes(std::make_shared<Meshes>()) {}
//GameObject::GameObject(const std::string &modelFilepath) : meshes(loadMeshes(modelFilepath)) {}
//GameObject::GameObject(const std::vector<float> &positions,
//                       const std::vector<float> &normals,
//                       const std::vector<float> &textureCoords,
//                       const std::vector<unsigned int> &indices,
//                       const std::string &textureFilepath) : meshes(std::make_shared<Meshes>()) {
//    meshes->push_back(std::make_unique<ge::Mesh>(positions, normals, textureCoords, indices, textureFilepath));
//}

void GameObject::onUpdate(std::chrono::duration<float> updateDuration) {}

void GameObject::render(ShaderProgram *shader) {
    this->model.render(shader);

    shader->setUniform("material.specularExponent", this->specularExponent);

    for (auto& mesh : *this->meshes) {
        mesh.render(shader);
    }
}

void GameObject::setMesh(std::shared_ptr<Meshes> mesh) {
    this->meshes = std::move(mesh);
}

void GameObject::setScale(const glm::vec3& scale) {
    this->model.setScale(scale);
}

void GameObject::setSpecularExponent(float specularExponent) {
    this->specularExponent = specularExponent;
}

} // namespace age