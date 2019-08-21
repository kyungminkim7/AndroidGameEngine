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
/// Initializes the game engine and environment. This should be called before creating a Game
/// instance and passing it to onCreate().
/// \param env JNI environment
/// \param windowWidth
/// \param windowHeight
/// \param j_asset_manager Android asset manager
///
void init(JNIEnv *env, int windowWidth, int windowHeight, jobject j_asset_manager);
void onCreate(std::unique_ptr<Game> game);
Game* getGame();

extern "C" {
JNI_METHOD_DECLARATION(void, onStart)(JNIEnv *env, jclass);
JNI_METHOD_DECLARATION(void, onResume)(JNIEnv *env, jclass);
JNI_METHOD_DECLARATION(void, onPause)(JNIEnv *env, jclass);
JNI_METHOD_DECLARATION(void, onStop)(JNIEnv *env, jclass);
JNI_METHOD_DECLARATION(void, onDestroy)(JNIEnv *env, jclass);
JNI_METHOD_DECLARATION(void, onSurfaceChanged)(JNIEnv *env, jclass, int width, int height);
JNI_METHOD_DECLARATION(void, update)(JNIEnv *env, jclass);
JNI_METHOD_DECLARATION(void, render)(JNIEnv *env, jclass);

JNI_METHOD_DECLARATION(void, onTouchDownEvent)(JNIEnv *env, jclass, float x, float y);
JNI_METHOD_DECLARATION(void, onTouchMoveEvent)(JNIEnv *env, jclass, float x, float y);
JNI_METHOD_DECLARATION(void, onTouchUpEvent)(JNIEnv *env, jclass, float x, float y);
}

} // namespace GameEngineJNI
} // namespace age

#include "GameEngineJNI_impl.h"