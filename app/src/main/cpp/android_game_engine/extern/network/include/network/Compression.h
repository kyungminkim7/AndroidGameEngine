#pragma once

#include <cstdint>
#include <memory>

#include <flatbuffers/flatbuffers.h>

namespace ntwk {

struct Image;

namespace Compression {
struct IdentityPolicy {
    static std::shared_ptr<flatbuffers::DetachedBuffer> compressMsg(std::shared_ptr<flatbuffers::DetachedBuffer> msg);
    static std::unique_ptr<uint8_t[]> decompressMsg(std::unique_ptr<uint8_t[]> msgBuffer);
};

namespace Image {
struct IdentityPolicy {
    static std::shared_ptr<flatbuffers::DetachedBuffer> compressMsg(unsigned int width, unsigned int height,
                                                                    uint8_t channels, const uint8_t data[]);
    static std::unique_ptr<ntwk::Image> decompressMsg(std::unique_ptr<uint8_t[]> msgBuffer);
};

struct JpegPolicy {
    static std::shared_ptr<flatbuffers::DetachedBuffer> compressMsg(unsigned int width, unsigned int height,
                                                                    uint8_t channels, const uint8_t data[]);
    static std::unique_ptr<ntwk::Image> decompressMsg(std::unique_ptr<uint8_t[]> msgBuffer);
};
} // namespace Image
} // namespace Compression
} // namespace ntwk
