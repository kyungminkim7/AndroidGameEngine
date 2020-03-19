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

    Asset(Asset &&) = default;
    Asset& operator=(Asset &&) = default;
    
    size_t getLength() const;
    size_t getRemainingLength() const;
    
    int read(void *buffer, size_t count);
    
    template <typename T>
    int read(T *x);
    
    int seek(int offset, int whence);
    
private:
    AAsset *asset;
    size_t length;
};

inline size_t Asset::getLength() const {return this->length;}
inline size_t Asset::getRemainingLength() const {return AAsset_getRemainingLength(this->asset);}
inline int Asset::read(void *buf, size_t count) {return AAsset_read(this->asset, buf, count);}

template <typename T>
inline int Asset::read(T *x) {return AAsset_read(this->asset, x, sizeof(T));}

inline int Asset::seek(int offset, int whence) {return AAsset_seek(this->asset, offset, whence);}

} // namespace age