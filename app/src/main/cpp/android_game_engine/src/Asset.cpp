#include <android_game_engine/Asset.h>

namespace age {

Asset::Asset(AAsset *asset) : asset(asset), length(AAsset_getLength(asset)) {}

Asset::~Asset() {AAsset_close(this->asset);}

} // namespace age