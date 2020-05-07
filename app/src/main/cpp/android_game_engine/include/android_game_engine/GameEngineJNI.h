#pragma once

#include <chrono>
#include <memory>

#include <jni.h>

// Users should define JAVA_ACTIVITY in order to establish JNI comms btwn Java and C++ code
#ifndef JAVA_ACTIVITY
#define JAVA_ACTIVITY GameActivity
#endif

#define CONCAT(a,b,c) a ## _ ## b ## _ ## c
#define EVAL_CONCAT(a,b,c) CONCAT(a,b,c)

#define JNI_METHOD_DEFINITION(return_type, method_name) \
  EVAL_CONCAT(return_type JNICALL Java_com_example_androidgameengine, JAVA_ACTIVITY, method_name)

#define JNI_METHOD_DECLARATION(return_type, method_name) \
  JNIEXPORT JNI_METHOD_DEFINITION(return_type, method_name)

/**
* Singleton game manager for native C++ development with JNI.
*/

namespace age {

class GameTemplate;

namespace GameEngineJNI {

///
/// Initializes the game engine and environment. This should be called before creating a Game
/// instance and passing it to onCreate().
/// \param env JNI environment
/// \param windowWidth
/// \param windowHeight
/// \param j_asset_manager Android asset manager
///
void init(JNIEnv *env, int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager);
void onCreate(std::unique_ptr<GameTemplate> game);
GameTemplate* getGame();

extern "C" {
JNI_METHOD_DECLARATION(void, onStartJNI)(JNIEnv *env, jobject);
JNI_METHOD_DECLARATION(void, onResumeJNI)(JNIEnv *env, jobject);
JNI_METHOD_DECLARATION(void, onPauseJNI)(JNIEnv *env, jobject);
JNI_METHOD_DECLARATION(void, onStopJNI)(JNIEnv *env, jobject);
JNI_METHOD_DECLARATION(void, onDestroyJNI)(JNIEnv *env, jobject);
JNI_METHOD_DECLARATION(void, onSurfaceChangedJNI)(JNIEnv *env, jobject, int width, int height, int displayRotation);
JNI_METHOD_DECLARATION(void, updateJNI)(JNIEnv *env, jobject);
JNI_METHOD_DECLARATION(void, renderJNI)(JNIEnv *env, jobject);

JNI_METHOD_DECLARATION(void, onTouchDownEventJNI)(JNIEnv *env, jobject, float x, float y);
JNI_METHOD_DECLARATION(void, onTouchMoveEventJNI)(JNIEnv *env, jobject, float x, float y);
JNI_METHOD_DECLARATION(void, onTouchUpEventJNI)(JNIEnv *env, jobject, float x, float y);
}

} // namespace GameEngineJNI
} // namespace age

#include "GameEngineJNI_impl.h"