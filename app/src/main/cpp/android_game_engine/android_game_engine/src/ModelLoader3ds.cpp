#include <android_game_engine/ModelLoader3ds.h>

#include <set>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/Exception.h>
#include <android_game_engine/ManagerAssets.h>
#include <android_game_engine/VertexBufferObject.h>

namespace {

struct Material {
    std::string diffuseTextureFilename;
    std::string specularTextureFilename;
};

std::unordered_map<std::string, std::weak_ptr<age::ModelLoader3ds::Meshes>> cachedMeshes;

long assetio_seek_func(void *self, long offset, Lib3dsIoSeek origin) {
    auto asset = reinterpret_cast<age::Asset*>(self);
    int o;
    switch (origin) {
        case LIB3DS_SEEK_SET:
            o = SEEK_SET;
            break;
        
        case LIB3DS_SEEK_CUR:
            o = SEEK_CUR;
            break;
        
        case LIB3DS_SEEK_END:
            o = SEEK_END;
            break;
        
        default:
            assert(0);
            return(0);
    }
    return asset->seek(offset, o);
}

long assetio_tell_func(void *self) {
    auto asset = reinterpret_cast<age::Asset*>(self);
    return asset->getLength() - asset->getRemainingLength();
}

size_t assetio_read_func(void *self, void *buffer, size_t size) {
    auto asset = reinterpret_cast<age::Asset*>(self);
    return asset->read(buffer, size);
}

Lib3dsFile* lib3ds_asset_open(const std::string &filepath) {
    auto asset = age::ManagerAssets::openAsset(filepath);
    
    auto lib3dsFile = lib3ds_file_new();
    if (!lib3dsFile) {
        throw age::LoadError("Failed to create new lib3ds file for model: " + filepath);
    }
    
    Lib3dsIo io;
    memset(&io, 0, sizeof(io));
    io.self = &asset;
    io.seek_func = assetio_seek_func;
    io.tell_func = assetio_tell_func;
    io.read_func = assetio_read_func;
    io.write_func = nullptr;
    io.log_func = nullptr;
    
    if (!lib3ds_file_read(lib3dsFile, &io)) {
        free(lib3dsFile);
        throw age::LoadError("Failed to read 3DS model file for: " + filepath);
    }
    
    return lib3dsFile;
}

} // namespace

namespace age {

ModelLoader3ds::ModelLoader3ds(const std::string &filepath)
    : ModelLoader(filepath), lib3dsFile(lib3ds_asset_open(filepath)) {}

ModelLoader3ds::~ModelLoader3ds() {
    lib3ds_file_free(this->lib3dsFile);
}

std::shared_ptr<ModelLoader3ds::Meshes> ModelLoader3ds::loadMeshes() {
    // Check cached meshes to avoid reloading
    auto meshes = cachedMeshes[this->getFilename()].lock();
    if (meshes) return meshes;
    
    auto filename = this->getFilename();
    auto meshesDeleter = [filename](auto meshes){
        // Clear cache
        cachedMeshes.erase(filename);
        delete meshes;
    };
    meshes = std::shared_ptr<Meshes>(new Meshes, meshesDeleter);
    
    // Extract mesh data from lib3ds file
    for (auto i = 0; i < this->lib3dsFile->nmeshes; ++i) {
        auto mesh = this->lib3dsFile->meshes[i];
        
        // Extract materials
        std::vector<Material> materials;
        materials.reserve(this->lib3dsFile->nmaterials);
        for (auto j = 0; j < this->lib3dsFile->nmaterials; ++j) {
            auto m = this->lib3dsFile->materials[j];
            materials.push_back({m->texture1_map.name,
                                 m->specular_map.name});
        }
        
        // Copy position and texture coordinate information
        std::vector<glm::vec3> positions;
        positions.reserve(mesh->nvertices);
        
        std::vector<glm::vec2> textureCoords;
        textureCoords.reserve(mesh->nvertices);
        
        for (auto j = 0; j < mesh->nvertices; ++j) {
            positions.emplace_back(mesh->vertices[j][0],
                                   mesh->vertices[j][1],
                                   mesh->vertices[j][2]);
            textureCoords.emplace_back(mesh->texcos[j][0],
                                       mesh->texcos[j][1]);
        }
        
        std::vector<glm::uvec3> indices;
        indices.reserve(mesh->nfaces);
        
        std::vector<glm::vec3> normals(mesh->nvertices);
        
        std::set<std::string> diffuseTextures, specularTextures;
        
        for (auto j = 0; j < mesh->nfaces; ++j) {
            // Copy indices
            auto& face = mesh->faces[j];
            auto a = face.index[0];
            auto b = face.index[1];
            auto c = face.index[2];
            
            indices.emplace_back(a, b, c);
    
            // Calculate normals
            normals[a] = normals[b] = normals[c] =
                    glm::cross(positions[b] - positions[a],
                               positions[c] - positions[b]);
            
            // Extract texture filepaths
            auto diffuseTextureFilename = materials[face.material].diffuseTextureFilename;
            auto specularTextureFilename = materials[face.material].specularTextureFilename;
            
            if (!diffuseTextureFilename.empty()) {
                diffuseTextures.insert(this->getDirectory() + diffuseTextureFilename);
            }
            
            if (!specularTextureFilename.empty()) {
                specularTextures.insert(this->getDirectory() + specularTextureFilename);
            }
        }
        
        meshes->emplace_back(std::make_shared<VertexBufferObject>(positions,
                                                                  normals,
                                                                  textureCoords),
                             std::make_shared<ElementBufferObject>(indices),
                             diffuseTextures.empty() ? std::set<std::string>{"images/white.png"} : diffuseTextures,
                             specularTextures.empty() ? std::set<std::string>{"images/white.png"} : specularTextures);
    }
    
    return meshes;
}

} // namespace age