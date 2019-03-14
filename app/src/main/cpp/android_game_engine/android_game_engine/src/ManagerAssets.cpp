#include <android_game_engine/ManagerAssets.h>

#include <android_game_engine/Exception.h>

namespace {
AAssetManager *assetManager = nullptr;
}

namespace age {
namespace ManagerAssets {

void init(AAssetManager *manager) {
    assetManager = manager;
}

void shutdown() {
    assetManager = nullptr;
}

Asset openAsset(const std::string &filepath) {
    if (assetManager == nullptr) {
        throw LoadError("AssetProxy manager is uninitialized.");
    }
    
    return Asset(AAssetManager_open(assetManager, filepath.c_str(), AASSET_MODE_UNKNOWN));
}

} // namespace ManagerAssets
} // namespace age