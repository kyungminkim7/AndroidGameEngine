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

/**
 * Data read from an asset. This data structure was chosen to avoid the
 * extra cost of initialization after data allocation in std structures
 * such as std::vector.
 */
struct Asset {
    std::unique_ptr<uint8_t[]> data;
    int size;
};

namespace ManagerAssets {

void init(AAssetManager *manager);
void shutdown();

Asset readAsset(const std::string &filepath);

} // namespace ManagerAssets
} // namespace age