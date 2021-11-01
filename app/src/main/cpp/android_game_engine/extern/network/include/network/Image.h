#pragma once

#include <cstdint>
#include <memory>

#include <flatbuffers/flatbuffers.h>

namespace ntwk {

struct Image {
    unsigned int width;
    unsigned int height;
    uint8_t channels;
    std::unique_ptr<uint8_t[]> data;

    Image(unsigned int width, unsigned int height, uint8_t channels,
          std::unique_ptr<uint8_t[]> &&data);

    static std::shared_ptr<flatbuffers::DetachedBuffer> makeBuffer(unsigned int width, unsigned int height,
                                                                   uint8_t channels, const uint8_t data[]);
    static Image makeImage(const uint8_t buffer[]);
};

} // namespace ntwk
