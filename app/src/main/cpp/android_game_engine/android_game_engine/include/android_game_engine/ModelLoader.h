#pragma once

#include <memory>
#include <string>

#include <BulletCollision/CollisionShapes/btCollisionShape.h>

#include "Mesh.h"

namespace age {

///
/// \brief Base class for loading and parsing 3D model files.
///        Subclass ModelLoader to provide implementations for specific 3D file types.
///
class ModelLoader {
public:
    using Meshes = std::vector<Mesh>;
    
    ///
    /// Extracts and saves the directory and filename of the filepath.
    /// \param filepath
    ///
    explicit ModelLoader(const std::string &filepath);
    virtual ~ModelLoader() = default;
    
    ///
    /// Extract mesh data from the 3D model file. The mesh data is internally cached.
    /// \return mesh data
    ///
    virtual std::shared_ptr<Meshes> loadMeshes() = 0;
    
    ///
    /// Extract collision shape from the 3D model file. The collision shape is internally cached.
    /// \return collision data
    ///
    virtual std::unique_ptr<btCollisionShape> loadCollisionShape() = 0;

protected:
    std::string getFilepath() const;
    std::string getDirectory() const;
    std::string getFilename() const;

private:
    std::string filepath;
    std::string directory;
    std::string filename;
};

} // namespace age