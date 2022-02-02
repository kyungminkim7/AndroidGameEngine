#include <android_game_engine/ManagerAssets.h>

#include <android/asset_manager_jni.h>

#include <android_game_engine/Asset.h>
#include <android_game_engine/Exception.h>
#include <android_game_engine/ManagerJNI.h>

namespace {
jobject jAssetManagerGlobal;
AAssetManager *assetManager = nullptr;
}

namespace age {
namespace ManagerAssets {

void init(JNIEnv *env, jobject jAssetManager) {
    jAssetManagerGlobal = env->NewGlobalRef(jAssetManager);
    assetManager = AAssetManager_fromJava(env, jAssetManagerGlobal);
}

void shutdown() {
    assetManager = nullptr;
    ManagerJNI::getJNIEnv()->DeleteGlobalRef(jAssetManagerGlobal);
}

Asset openAsset(const std::string &filepath) {
    auto asset = AAssetManager_open(assetManager, filepath.c_str(), AASSET_MODE_UNKNOWN);
    if (asset == nullptr) {
        throw LoadError("Failed to open asset: " + filepath);
    }
    return Asset(asset);
}

} // namespace ManagerAssets
} // namespace age