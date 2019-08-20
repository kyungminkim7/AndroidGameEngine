#pragma once

/**
 * Singleton game manager for fully native C++ development. Users should subclass Game and then
 * call GameEngine::run with the template instantiation of the derived Game class.
 */

#include <android_native_app_glue.h>

namespace age {
namespace GameEngineNative {

template <typename T>
void run(android_app *app);

} // namespace GameEngineNative
} // namespace age

#include "GameEngineNative_impl.h"