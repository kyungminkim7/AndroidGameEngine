#pragma once

#include <chrono>
#include <memory>

#include <jni.h>

#include "Game.h"

#define JNI_METHOD_DEFINITION(return_type, method_name) \
  return_type JNICALL Java_com_example_androidgameengine_GameJNI_##method_name

#define JNI_METHOD_DECLARATION(return_type, method_name) \
  JNIEXPORT JNI_METHOD_DEFINITION(return_type, method_name)

/**
* Singleton game manager for native C++ development with JNI.
*/

namespace age {
namespace GameEngineJNI {

///
/// Initializes the game engine. This should be called before loadGame().
/// \param env JNI environment
/// \param windowWidth
/// \param windowHeight
/// \param j_asset_manager Android asset manager
///
void init(JNIEnv *env, int windowWidth, int windowHeight, jobject j_asset_manager);
void loadGame(std::unique_ptr<Game> game);
Game* getGame();

extern "C" {
JNI_METHOD_DECLARATION(void, setWindowSize)(JNIEnv *, jclass, int width, int height);
JNI_METHOD_DECLARATION(void, update)(JNIEnv *, jclass);
JNI_METHOD_DECLARATION(void, render)(JNIEnv *, jclass);
JNI_METHOD_DECLARATION(void, shutdown)(JNIEnv *, jclass);

JNI_METHOD_DECLARATION(void, onTouchDownEvent)(JNIEnv *, jclass, float x, float y);
JNI_METHOD_DECLARATION(void, onTouchMoveEvent)(JNIEnv *, jclass, float x, float y);
JNI_METHOD_DECLARATION(void, onTouchUpEvent)(JNIEnv *, jclass, float x, float y);
}

} // namespace GameEngineJNI
} // namespace age

#include "GameEngineJNI_impl.h"