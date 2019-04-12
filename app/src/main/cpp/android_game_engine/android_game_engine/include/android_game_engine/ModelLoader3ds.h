#pragma once

#include "ModelLoader.h"

#include <lib3ds.h>

namespace age {

///
/// \brief Loads 3DS model files.
///
class ModelLoader3ds : public ModelLoader {
public:
    explicit ModelLoader3ds(const std::string &filepath);
    
    std::shared_ptr<Meshes> loadMeshes() override;
    std::unique_ptr<btCollisionShape> loadCollisionShape() override;
    glm::vec3 loadDimensions() override;

private:
    std::unique_ptr<btCollisionShape> loadConvexHull();
    std::unique_ptr<btCollisionShape> loadCompoundShape();
    
    std::shared_ptr<Lib3dsFile> lib3dsFile;
};

} // namespace