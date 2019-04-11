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
    std::shared_ptr<btCollisionShape> loadCollisionShape() override;

private:
    std::shared_ptr<btCollisionShape> loadConvexHull();
    
    std::shared_ptr<Lib3dsFile> lib3dsFile;
};

} // namespace