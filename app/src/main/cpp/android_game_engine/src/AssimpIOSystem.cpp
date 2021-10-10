#include <android_game_engine/AssimpIOSystem.h>

#include <android_game_engine/AssimpIOStream.h>
#include <android_game_engine/ManagerAssets.h>

namespace age {

bool AssimpIOSystem::Exists(const char *pathname) const {
    try {
        ManagerAssets::openAsset(pathname);
    } catch (...) {
        return false;
    }
    return true;
}

char AssimpIOSystem::getOsSeparator() const {
    return '/';
}

Assimp::IOStream *AssimpIOSystem::Open(const char *file, const char *mode) {
    return new AssimpIOStream(file);
}

void AssimpIOSystem::Close(Assimp::IOStream *file) {
    delete file;
}

} // namespace age