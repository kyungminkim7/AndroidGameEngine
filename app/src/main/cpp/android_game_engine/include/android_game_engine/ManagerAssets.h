#pragma once

/**
 * Singleton asset manager for loading data from the assets directory.
 * Initialize ManagerAssets after ManagerWindowing.
 * Shutdown ManagerAssets before ManagerWindowing.
 */

#include <cstdint>
#include <memory>
#include <string>

#include <android/asset_manager.h>

namespace age {

class Asset;

namespace ManagerAssets {

void init(AAssetManager *manager);
void shutdown();

Asset openAsset(const std::string &filepath);

} // namespace ManagerAssets
} // namespace age