#pragma once

#include <android_native_app_glue.h>

namespace age {
namespace GameEngine {

template <typename T>
void run(android_app *app);

} // namespace GameEngine
} // namespace age

#include "GameEngine_impl.h"