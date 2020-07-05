#pragma once

#include <cstdint>
#include <memory>

#include <flatbuffers/flatbuffers.h>

namespace ntwk {

struct Image;

enum class Compression {NONE, JPEG};

namespace jpeg {

std::shared_ptr<flatbuffers::DetachedBuffer> encodeMsg(unsigned int width, unsigned int height,
                                                       uint8_t channels, const uint8_t data[]);
std::unique_ptr<Image> decodeMsg(const uint8_t jpegMsgBuffer[]);


} // namespace jpeg

} // namespace ntwk
