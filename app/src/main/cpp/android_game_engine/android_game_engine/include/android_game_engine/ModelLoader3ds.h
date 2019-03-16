#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Asset.h"
#include "Mesh.h"

namespace age {

class ModelLoader3ds {
public:
    static std::shared_ptr<std::vector<Mesh>> loadModel(const std::string &modelFilepath);

protected:
    explicit ModelLoader3ds(const std::string &modelFilepath);
    
private:
    struct Material;
    
    std::vector<Mesh>* parseModel();
    void parseEdit();
    void parseObject();
    void parseMesh();
    void parseFaces();
    void parseMaterial();
    void parseTextureMap();
    void parseSpecularMap();
    
    int readChunkHeader();
    std::string readString();
    void skipChunk();
    
    struct ChunkHeader {
        uint16_t id;
        uint32_t length;
    };
    
    struct Node {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textureCoords;
        std::vector<glm::uvec3> indices;
        std::set<std::string> diffuseTextures;
        std::set<std::string> specularTextures;
    };
    
    struct Material {
        std::string name;
        std::string diffuseTextureFilename;
        std::string specularTextureFilename;
    };
    
    Asset asset;
    std::string modelDirectory;
    
    ChunkHeader currentChunk;
    
    std::vector<Node> nodes;
    std::vector<Material> materials;
};

} // namespace