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

Asset readAsset(const std::string &filepath) {
    if (assetManager == nullptr) {
        throw LoadError("Asset manager is uninitialized.");
    }
    
    auto asset = AAssetManager_open(assetManager, filepath.c_str(), AASSET_MODE_UNKNOWN);
    auto size = AAsset_getLength(asset);
    Asset result{std::unique_ptr<uint8_t[]>(new uint8_t[size]), static_cast<int>(size)};
    auto numBytesRead = AAsset_read(asset, result.data.get(), static_cast<int>(size));
    AAsset_close(asset);
    
    if (numBytesRead < 0) {
        throw LoadError("Failed to read asset data from file: " + filepath);
    } else {
        return result;
    }
}

} // namespace ManagerAssets
} // namespace age