#pragma once

#include <type_traits>

namespace ntwk {

template<typename Enum>
constexpr auto toUnderlyingType(Enum e) {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

} // namespace ntwk
