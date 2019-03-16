#include <android_game_engine/ModelLoader3ds.h>

#include <algorithm>
#include <memory>
#include <sstream>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/ElementBufferObject.h>
#include <android_game_engine/Exception.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerAssets.h>
#include <android_game_engine/VertexBufferObject.h>

namespace {

namespace ChunkId {
constexpr uint16_t MAIN = 0x4D4D;

    constexpr uint16_t VERSION = 0x0002;
    
    constexpr uint16_t EDIT = 0x3D3D;
        constexpr uint16_t EDIT_OBJECT = 0x4000;
            constexpr uint16_t OBJECT_MESH = 0x4100;
                constexpr uint16_t MESH_VERTICES = 0x4110;
                
                constexpr uint16_t MESH_FACES = 0x4120;
                    constexpr uint16_t FACES_MATERIAL = 0x4130;
                
                constexpr uint16_t MESH_MAP_COORDS = 0x4140;
                
                constexpr uint16_t MESH_LOCAL_COORDS = 0x4160;
        
        constexpr uint16_t EDIT_MATERIAL = 0xAFFF;
            constexpr uint16_t MATERIAL_NAME = 0xA000;
            
            constexpr uint16_t MATERIAL_AMBIENT_COLOR = 0xA010;
            
            constexpr uint16_t MATERIAL_DIFFUSE_COLOR = 0xA020;
            
            constexpr uint16_t MATERIAL_SPECULAR_COLOR = 0xA030;
            
            constexpr uint16_t MATERIAL_SHININESS = 0xA040;
            
            constexpr uint16_t MATERIAL_TEXTURE_MAP = 0xA200;
            
            constexpr uint16_t MATERIAL_SPECULAR_MAP = 0xA204;
            
            constexpr uint16_t MATERIAL_BUMP_MAP = 0xA230;
            
            constexpr uint16_t MATERIAL_REFLECTION_MAP = 0xA220;
            
                constexpr uint16_t MAP_FILENAME = 0xA300;
                
                constexpr uint16_t MAP_PARAMETERS = 0xA351;
    constexpr uint16_t KEYFRAMER = 0xB000;
} // namespace ChunkId

constexpr auto chunkHeaderSize = sizeof(uint16_t) + sizeof(uint32_t);

std::unordered_map<std::string, std::weak_ptr<std::vector<age::Mesh>>> cachedMeshes;

} // namespace

namespace age {

std::shared_ptr<std::vector<Mesh>> ModelLoader3ds::loadModel(const std::string &modelFilepath) {
    auto modelFilenameIndex = modelFilepath.find_last_of('/');
    const auto modelDirectory = modelFilepath.substr(0, modelFilenameIndex);
    const auto modelFilename = modelFilepath.substr(modelFilenameIndex + 1);
    
    // Check cached meshes to avoid reloading
    auto meshes = cachedMeshes[modelFilename].lock();
    if (meshes) return meshes;
    
    // Parse 3ds file
    ModelLoader3ds loader(modelFilepath);
    
    auto meshesDeleter = [modelFilename](auto meshes){
        // Clear cache
        cachedMeshes.erase(modelFilename);
        delete meshes;
    };
    meshes = std::shared_ptr<std::vector<Mesh>>(loader.parseModel(), meshesDeleter);
    
    Log::info("Successfully loaded model from file: " + modelFilepath);
    cachedMeshes[modelFilename] = meshes;
    return meshes;
}

ModelLoader3ds::ModelLoader3ds(const std::string &modelFilepath)
    : asset(ManagerAssets::openAsset(modelFilepath)) {
    auto pos = modelFilepath.find_last_of('/');
    this->modelDirectory = modelFilepath.substr(0, pos + 1);
}
    
std::vector<Mesh>* ModelLoader3ds::parseModel() {
    auto n = this->readChunkHeader();
    
    if (this->currentChunk.id != ChunkId::MAIN) {
        throw age::LoadError("Failed to parse MAIN chunk from 3ds file.");
    }
    
    auto length = this->currentChunk.length;
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
        
        switch (this->currentChunk.id) {
            case ChunkId::EDIT:
                parseEdit();
                break;
            
            default:
                this->skipChunk();
                break;
        }
    }
    
    auto meshes = new std::vector<Mesh>;
    for (const auto& node : this->nodes) {
        meshes->emplace_back(std::make_shared<VertexBufferObject>(node.positions,
                                                                  node.normals,
                                                                  node.textureCoords),
                             std::make_shared<ElementBufferObject>(node.indices),
                             node.diffuseTextures.empty() ? std::set<std::string>{"images/white.png"} : node.diffuseTextures,
                             node.specularTextures.empty() ? std::set<std::string>{"images/white.png"} : node.specularTextures);
    }
    return meshes;
}

void ModelLoader3ds::parseEdit() {
    auto n = chunkHeaderSize;
    auto length = this->currentChunk.length;
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
        
        switch (this->currentChunk.id) {
            case ChunkId::EDIT_OBJECT:
                this->nodes.push_back({});
                parseObject();
                break;
            
            case ChunkId::EDIT_MATERIAL:
                this->parseMaterial();
                break;
            
            default:
                this->skipChunk();
                break;
        }
    }
}

void ModelLoader3ds::parseObject() {
    auto n = chunkHeaderSize;
    auto length = this->currentChunk.length;
    
    auto objectName = this->readString();
    n += objectName.length();
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
        
        switch (this->currentChunk.id) {
            case ChunkId::OBJECT_MESH:
                this->parseMesh();
                break;
            
            default:
                this->skipChunk();
                break;
        }
    }
}

void ModelLoader3ds::parseMesh() {
    auto n = chunkHeaderSize;
    auto length = this->currentChunk.length;
    
    auto& positions = this->nodes.back().positions;
    auto& normals = this->nodes.back().normals;
    auto& textureCoords = this->nodes.back().textureCoords;
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
        
        switch (this->currentChunk.id) {
            case ChunkId::MESH_VERTICES:
                uint16_t numVertices;
                this->asset.read(&numVertices);
                
                positions.resize(numVertices);
                normals.resize(numVertices);
                
                this->asset.read(positions.data(), numVertices * sizeof(glm::vec3));
                break;
            
            case ChunkId::MESH_FACES:
                parseFaces();
                break;
            
            case ChunkId::MESH_MAP_COORDS:
                uint16_t numTextureCoords;
                this->asset.read(&numTextureCoords);
                
                textureCoords.resize(numTextureCoords);
                
                this->asset.read(textureCoords.data(), numTextureCoords * sizeof(glm::vec2));
                break;
            
            default:
                this->skipChunk();
                break;
        }
    }
}

void ModelLoader3ds::parseFaces() {
    auto n = chunkHeaderSize;
    auto length = this->currentChunk.length;
    
    auto& positions = this->nodes.back().positions;
    auto& normals = this->nodes.back().normals;
    auto& indices = this->nodes.back().indices;
    
    uint16_t numFaces;
    n += this->asset.read(&numFaces);
    indices.reserve(numFaces);
    
    uint16_t a, b, c, flag;
    for (auto i=0u; i < numFaces; ++i) {
        // Read indices
        n += this->asset.read(&a);
        n += this->asset.read(&b);
        n += this->asset.read(&c);
        indices.push_back(glm::uvec3(a, b, c));
        
        n += this->asset.read(&flag);
        
        // Calculate normal
        normals[a] = normals[b] = normals[c] =
                glm::cross(positions[b] - positions[a],
                           positions[c] - positions[b]);
    }
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
        
        switch (this->currentChunk.id) {
            case ChunkId::FACES_MATERIAL: {
                auto materialName = this->readString();
    
                // ID what kind of texture this material is
                auto iter = std::find_if(this->materials.begin(), this->materials.end(),
                        [materialName](const auto& m){return m.name == materialName;});
                
                if (iter != this->materials.end()) {
                    if (!iter->diffuseTextureFilename.empty()) {
                        this->nodes.back().diffuseTextures.insert(this->modelDirectory + iter->diffuseTextureFilename);
                    }
                    
                    if (!iter->specularTextureFilename.empty()) {
                        this->nodes.back().specularTextures.insert(this->modelDirectory + iter->specularTextureFilename);
                    }
                }
    
                // Skip over rest of the chunk
                uint16_t numFaces, faceIndex;
                this->asset.read(&numFaces);
                this->asset.seek(numFaces * sizeof(faceIndex), SEEK_CUR);
                break;
            }
            
            default:
                this->skipChunk();
                break;
        }
    }
}

void ModelLoader3ds::parseMaterial() {
    auto n = chunkHeaderSize;
    auto length = this->currentChunk.length;
    
    this->materials.push_back({});
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
    
        switch (this->currentChunk.id) {
            case ChunkId::MATERIAL_NAME:
                this->materials.back().name = this->readString();
                break;
            
            case ChunkId::MATERIAL_TEXTURE_MAP:
                this->parseTextureMap();
                break;
    
            case ChunkId::MATERIAL_SPECULAR_MAP:
                this->parseSpecularMap();
                break;
            
            default:
                this->skipChunk();
                break;
        }
    }
}

void ModelLoader3ds::parseTextureMap() {
    auto n = chunkHeaderSize;
    auto length = this->currentChunk.length;
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
        
        switch (this->currentChunk.id) {
            case ChunkId::MAP_FILENAME:
                this->materials.back().diffuseTextureFilename = this->readString();
                break;
            
            default:
                this->skipChunk();
                break;
        }
    }
}

void ModelLoader3ds::parseSpecularMap(){
    auto n = chunkHeaderSize;
    auto length = this->currentChunk.length;
    
    while (n < length) {
        this->readChunkHeader();
        n += this->currentChunk.length;
        
        switch (this->currentChunk.id) {
            case ChunkId::MAP_FILENAME:
                this->materials.back().specularTextureFilename = this->readString();
                break;
            
            default:
                this->skipChunk();
                break;
        }
    }
}

int ModelLoader3ds::readChunkHeader() {
    return this->asset.read(&this->currentChunk.id) + this->asset.read(&this->currentChunk.length);
}

std::string ModelLoader3ds::readString() {
    std::stringstream ss;
    char ch;
    do {
        this->asset.read(&ch);
        ss.put(ch);
    } while (ch != '\0');
    return ss.str();
}

void ModelLoader3ds::skipChunk() {
    this->asset.seek(this->currentChunk.length - chunkHeaderSize, SEEK_CUR);
}

} // namespace age