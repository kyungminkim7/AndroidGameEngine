#include <android_game_engine/AssimpIOStream.h>

#include <android_game_engine/ManagerAssets.h>

namespace age {

AssimpIOStream::AssimpIOStream(const std::string &pathname) :
    asset(ManagerAssets::openAsset(pathname)) {}

size_t AssimpIOStream::Read(void *buffer, size_t size, size_t count) {
    return this->asset.read(buffer, size * count);
}

size_t AssimpIOStream::Write(const void *buffer, size_t size, size_t count) {
    return -1;
}

aiReturn AssimpIOStream::Seek(size_t offset, aiOrigin origin) {
    int whence;
    switch (origin) {
        case aiOrigin_SET: whence = SEEK_SET; break;
        case aiOrigin_CUR: whence = SEEK_CUR; break;
        case aiOrigin_END: whence = SEEK_END; break;
        default: return aiReturn_FAILURE;
    }

    auto result = this->asset.seek(offset, whence);
    return (result == -1) ? aiReturn_FAILURE : aiReturn_SUCCESS;
}

size_t AssimpIOStream::Tell() const {
    return this->asset.seek(0, SEEK_CUR);
}

size_t AssimpIOStream::FileSize() const {
    return this->asset.getLength();
}

void AssimpIOStream::Flush() {}



} // namespace age
