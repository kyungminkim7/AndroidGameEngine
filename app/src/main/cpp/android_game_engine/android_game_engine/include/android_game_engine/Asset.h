#pragma once

#include <android/asset_manager.h>

namespace age {

///
/// Wrapper class for android AAsset. Takes ownership of the AAsset and closes it on destruction.
///
class Asset {
public:
    ///
    /// Takes ownership of the AAsset and closes it on destruction.
    /// \param asset
    ///
    explicit Asset(AAsset *asset);
    ~Asset();
    
    size_t getLength() const;
    size_t getRemainingLength() const;
    
    int read(void *buffer, size_t count);
    int seek(int offset, int origin);
    
private:
    AAsset *asset;
    size_t length;
};

inline size_t Asset::getLength() const {return this->length;}
inline size_t Asset::getRemainingLength() const {return AAsset_getRemainingLength(this->asset);}
inline int Asset::read(void *buf, size_t count) {return AAsset_read(this->asset, buf, count);}
inline int Asset::seek(int offset, int origin) {return AAsset_seek(this->asset, offset, origin);}

} // namespace age