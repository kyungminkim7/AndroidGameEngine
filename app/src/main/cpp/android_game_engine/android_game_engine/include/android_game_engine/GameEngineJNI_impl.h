#include <android/asset_manager_jni.h>

#include "Log.h"
#include "ManagerAssets.h"
#include "ManagerWindowing.h"

namespace {

std::unique_ptr<age::Game> game = nullptr;

} // namespace

namespace age {
namespace GameEngineJNI {

void init(JNIEnv *env, int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    ManagerWindowing::init(windowWidth, windowHeight, displayRotation);
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

JNI_METHOD_DEFINITION(void, onStartJNI)(JNIEnv *env, jobject) {
    if (game) game->onStart();
}

JNI_METHOD_DEFINITION(void, onResumeJNI)(JNIEnv *env, jobject) {
    if (game) game->onResume();
}

JNI_METHOD_DEFINITION(void, onPauseJNI)(JNIEnv *env, jobject) {
    game->onPause();
}

JNI_METHOD_DEFINITION(void, onStopJNI)(JNIEnv *env, jobject) {
    game->onStop();
}

JNI_METHOD_DEFINITION(void, onDestroyJNI)(JNIEnv *env, jobject) {
    game->onDestroy();
    game.reset();
    ManagerAssets::shutdown();
    ManagerWindowing::shutdown();
}

JNI_METHOD_DEFINITION(void, onSurfaceChangedJNI)(JNIEnv *env, jobject, int width, int height, int displayRotation) {
    ManagerWindowing::setWindowDimensions(width, height);
    ManagerWindowing::setDisplayRotation(displayRotation);

    game->onWindowChanged(width, height, displayRotation);
}

JNI_METHOD_DEFINITION(void, updateJNI)(JNIEnv *env, jobject) {
    static std::chrono::system_clock::time_point lastUpdateTime = std::chrono::system_clock::now();

    auto currentUpdateTime = std::chrono::system_clock::now();
    auto updateDuration = currentUpdateTime - lastUpdateTime;
    lastUpdateTime = currentUpdateTime;

    game->onUpdate(updateDuration);
}

JNI_METHOD_DEFINITION(void, renderJNI)(JNIEnv *env, jobject) {
    game->render();
}

JNI_METHOD_DEFINITION(void, onTouchDownEventJNI)(JNIEnv *env, jobject, float x, float y) {
    game->onTouchDownEvent(x, y);
}

JNI_METHOD_DEFINITION(void, onTouchMoveEventJNI)(JNIEnv *env, jobject, float x, float y) {
    game->onTouchMoveEvent(x, y);
}

JNI_METHOD_DEFINITION(void, onTouchUpEventJNI)(JNIEnv *env, jobject, float x, float y) {
    game->onTouchUpEvent(x, y);
}

} // namespace GameEngineJNI
} // namespace age