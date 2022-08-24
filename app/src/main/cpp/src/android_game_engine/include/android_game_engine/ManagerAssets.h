#pragma once

/**
 * Singleton asset manager for loading data from the assets directory.
 * Initialize ManagerAssets after ManagerWindowing.
 * Shutdown ManagerAssets before ManagerWindowing.
 */

#include <memory>
#include <string>

#include <jni.h>

#include <android/asset_manager.h>

namespace age {

class Asset;

namespace ManagerAssets {

void init(JNIEnv *env, jobject jAssetManager);
void shutdown();

Asset openAsset(const std::string &filepath);

} // namespace ManagerAssets
} // namespace age