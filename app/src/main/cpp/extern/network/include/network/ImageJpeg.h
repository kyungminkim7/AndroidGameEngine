#pragma once

#include <cstdint>
#include <memory>

#include <flatbuffers/flatbuffers.h>

#include "Image.h"

namespace ntwk {
namespace ImageJpeg {

std::shared_ptr<flatbuffers::DetachedBuffer> compressImage(unsigned int width, unsigned int height,
                                                           uint8_t channels, const uint8_t data[],
                                                           int quality=80);

Image decompressImage(const uint8_t jpegBuffer[]);

} // namespace ImageJpeg
} // namespace ntwk
