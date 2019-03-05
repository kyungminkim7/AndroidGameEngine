#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <android/asset_manager.h>

namespace age {

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