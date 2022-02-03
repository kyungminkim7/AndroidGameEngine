#pragma once

#include <memory>
#include <mutex>

#include <jni.h>

/**
* Singleton game manager
*/

namespace age {

class Game;

namespace GameEngine {

std::unique_lock<std::mutex> getGameLock();
Game *getGame();

void onCreate(JNIEnv *env, jobject assetManager);
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