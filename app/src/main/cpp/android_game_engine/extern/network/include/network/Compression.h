#pragma once

#include <cstdint>
#include <memory>

#include <flatbuffers/flatbuffers.h>

namespace ntwk {

struct Image;

enum class Compression {NONE, ZLIB, JPEG};

namespace zlib {

std::shared_ptr<flatbuffers::DetachedBuffer> encodeMsg(flatbuffers::DetachedBuffer *msg);
std::unique_ptr<uint8_t[]> decodeMsg(uint8_t compressedMsgBuffer[]);

} // namespace zlib

namespace jpeg {

std::shared_ptr<flatbuffers::DetachedBuffer> encodeMsg(unsigned int width, unsigned int height,
                                                       uint8_t channels, const uint8_t data[]);
std::unique_ptr<Image> decodeMsg(const uint8_t jpegMsgBuffer[]);


} // namespace jpeg

} // namespace ntwk