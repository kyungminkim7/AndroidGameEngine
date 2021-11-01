#pragma once

#include <cstdint>

namespace ntwk {

enum class MsgTypeId : uint32_t {
    IMAGE,
    IMAGE_JPEG,
    JOYSTICK,
    MSG_CTRL,
    TWIST,
    UINT8_ARRAY,
    VECTOR3
};

} // namespace ntwk
