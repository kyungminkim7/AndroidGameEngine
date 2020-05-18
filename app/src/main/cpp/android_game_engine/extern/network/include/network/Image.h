#pragma once

#include <cstdint>
#include <memory>

namespace ntwk {

struct Image {
    unsigned int width;
    unsigned int height;
    uint8_t channels;
    std::unique_ptr<uint8_t[]> data;
};

} // namespace ntwk
