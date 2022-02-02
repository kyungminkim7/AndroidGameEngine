#pragma once

#include <chrono>
#include <memory>
#include <mutex>

#include <jni.h>

#include "Game.h"

/**
* Singleton game manager and main entry point for native C++ development with JNI.
*/

#define CONCAT(a,b,c) a ## _ ## b ## _ ## c
#define EVAL_CONCAT(a,b,c) CONCAT(a,b,c)

#define JNI_METHOD_DEFINITION(return_type, method_name) \
  EVAL_CONCAT(return_type JNICALL Java_com_example_androidgameengine, JAVA_ACTIVITY, method_name)

#define JNI_METHOD_DECLARATION(return_type, method_name) \
  JNIEXPORT JNI_METHOD_DEFINITION(return_type, method_name)

namespace age {
namespace GameEngine {

std::unique_lock<std::mutex> getGameLock();
Game *getGame();

void onCreate(JNIEnv *env, jobject activity, jobject context, jobject assetManager);
void onStart();
void onResume();
void onPause();
void onStop();
void onDestroy();

void onSurfaceCreated(int width, int height, int displayRotation, std::unique_ptr<Game> &&g);
void onSurfaceChanged(int width, int height, int displayRotation);

void update();
void onTouchDownEvent(float x, float y);
void onTouchMoveEvent(float x, float y);
void onTouchUpEvent(float x, float y);

} // namespace GameEngine
} // namespace age

/**
 * JNI calls
 */
extern "C" {

JNI_METHOD_DECLARATION(void, onCreateJNI)(JNIEnv *env, jobject activity,
                                          jobject context, jobject assetManager) {
  age::GameEngine::onCreate(env, activity, context, assetManager);
}

JNI_METHOD_DECLARATION(void, onStartJNI)(JNIEnv *env, jobject activity) {
  age::GameEngine::onStart();
}

JNI_METHOD_DECLARATION(void, onResumeJNI)(JNIEnv *env, jobject activity) {
  age::GameEngine::onResume();
}

JNI_METHOD_DECLARATION(void, onPauseJNI)(JNIEnv *env, jobject activity) {
  age::GameEngine::onPause();
}

JNI_METHOD_DECLARATION(void, onStopJNI)(JNIEnv *env, jobject activity) {
  age::GameEngine::onStop();
}

JNI_METHOD_DECLARATION(void, onDestroyJNI)(JNIEnv *env, jobject activity) {
  age::GameEngine::onDestroy();
}

JNI_METHOD_DECLARATION(void, onSurfaceChangedJNI)(JNIEnv *env, jobject activity,
                                                  int width, int height, int displayRotation) {
  age::GameEngine::onSurfaceChanged(width, height, displayRotation);
}

JNI_METHOD_DECLARATION(void, updateJNI)(JNIEnv *env, jobject activity) {
  age::GameEngine::update();
}

JNI_METHOD_DECLARATION(void, onTouchDownEventJNI)(JNIEnv *env, jobject activity,
                                                  float x, float y) {
  age::GameEngine::onTouchDownEvent(x, y);
}

JNI_METHOD_DECLARATION(void, onTouchMoveEventJNI)(JNIEnv *env, jobject activity,
                                                  float x, float y) {
  age::GameEngine::onTouchMoveEvent(x, y);
}

JNI_METHOD_DECLARATION(void, onTouchUpEventJNI)(JNIEnv *env, jobject activity,
                                                float x, float y) {
  age::GameEngine::onTouchUpEvent(x, y);
}

} // extern "C"

