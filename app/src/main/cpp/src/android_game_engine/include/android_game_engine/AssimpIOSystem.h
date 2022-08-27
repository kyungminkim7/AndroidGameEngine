#pragma once

#include <assimp/IOSystem.hpp>

#include "Asset.h"

namespace age {

class AssimpIOSystem : public Assimp::IOSystem {
public:
    bool Exists(const char *pathname) const override;
    char getOsSeparator() const override;
    Assimp::IOStream *Open(const char *file, const char *mode) override;
    void Close(Assimp::IOStream *file) override;
};

} // namespace age