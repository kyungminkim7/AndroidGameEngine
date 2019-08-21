#include <android/asset_manager_jni.h>

#include "Log.h"
#include "ManagerAssets.h"
#include "ManagerWindowing.h"

namespace age {
namespace GameEngineJNI {

std::unique_ptr<Game> game;

std::chrono::system_clock::time_point lastUpdateTime;

void init(JNIEnv *env, int windowWidth, int windowHeight, jobject j_asset_manager) {
    ManagerWindowing::init(windowWidth, windowHeight);
    ManagerAssets::init(AAssetManager_fromJava(env, j_asset_manager));
}

void onCreate(std::unique_ptr<Game> g) {
    game = std::move(g);
    game->onCreate();
    game->onStart();
    game->onResume();
}

Game* getGame() {
    return game.get();
}

JNI_METHOD_DEFINITION(void, onStart)(JNIEnv *env, jclass) {
    if (game) game->onStart();
}

JNI_METHOD_DEFINITION(void, onResume)(JNIEnv *env, jclass) {
    if (game) game->onResume();
}

JNI_METHOD_DEFINITION(void, onPause)(JNIEnv *env, jclass) {
    game->onPause();
}

JNI_METHOD_DEFINITION(void, onStop)(JNIEnv *env, jclass) {
    game->onStop();
}

JNI_METHOD_DEFINITION(void, onDestroy)(JNIEnv *env, jclass) {
    game->onDestroy();
    game.reset();
    ManagerAssets::shutdown();
    ManagerWindowing::shutdown();
}

JNI_METHOD_DEFINITION(void, onSurfaceChanged)(JNIEnv *env, jclass, int width, int height) {
    ManagerWindowing::setWindowDimensions(width, height);
    game->onWindowSizeChanged(width, height);
}

JNI_METHOD_DEFINITION(void, update)(JNIEnv *env, jclass) {
    auto currentUpdateTime = std::chrono::system_clock::now();
    auto updateDuration = currentUpdateTime - lastUpdateTime;
    lastUpdateTime = currentUpdateTime;

    game->onUpdate(updateDuration);
}

JNI_METHOD_DEFINITION(void, render)(JNIEnv *env, jclass) {
    game->render();
}

JNI_METHOD_DEFINITION(void, onTouchDownEvent)(JNIEnv *env, jclass, float x, float y) {
    game->onTouchDownEvent(x, y);
}

JNI_METHOD_DEFINITION(void, onTouchMoveEvent)(JNIEnv *env, jclass, float x, float y) {
    game->onTouchMoveEvent(x, y);
}

JNI_METHOD_DEFINITION(void, onTouchUpEvent)(JNIEnv *env, jclass, float x, float y) {
    game->onTouchUpEvent(x, y);
}

} // namespace GameEngineJNI
} // namespace age