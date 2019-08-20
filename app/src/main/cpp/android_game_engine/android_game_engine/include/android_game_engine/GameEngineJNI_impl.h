#include <android/asset_manager_jni.h>

#include "Log.h"
#include "ManagerAssets.h"
#include "ManagerWindowing.h"

namespace age {
namespace GameEngineJNI {

std::unique_ptr<Game> game;

std::chrono::system_clock::time_point lastUpdateTime;

void init(JNIEnv *env, int windowWidth, int windowHeight, jobject j_asset_manager) {
    age::Log::info("Initializing game engine.");
    ManagerWindowing::init(windowWidth, windowHeight);
    ManagerAssets::init(AAssetManager_fromJava(env, j_asset_manager));
}

void loadGame(std::unique_ptr<Game> g) {
    game = std::move(g);
    game->init();
    game->loadWorld();
}

Game* getGame() {
    return game.get();
}

JNI_METHOD_DEFINITION(void, setWindowSize)(JNIEnv *, jclass, int width, int height) {
    ManagerWindowing::setWindowDimensions(width, height);
    game->onWindowSizeChanged(width, height);
}

JNI_METHOD_DEFINITION(void, update)(JNIEnv *, jclass) {
    auto currentUpdateTime = std::chrono::system_clock::now();
    auto updateDuration = currentUpdateTime - lastUpdateTime;
    lastUpdateTime = currentUpdateTime;

    game->onUpdate(updateDuration);
}

JNI_METHOD_DEFINITION(void, render)(JNIEnv *, jclass) {
    game->render();
}

JNI_METHOD_DEFINITION(void, shutdown)(JNIEnv *, jclass) {
    age::Log::info("Shutting down game engine.");
    game.reset();
    ManagerAssets::shutdown();
    ManagerWindowing::shutdown();
}

JNI_METHOD_DEFINITION(void, onTouchDownEvent)(JNIEnv *, jclass, float x, float y) {
    game->onTouchDownEvent(x, y);
}

JNI_METHOD_DEFINITION(void, onTouchMoveEvent)(JNIEnv *, jclass, float x, float y) {
    game->onTouchMoveEvent(x, y);
}

JNI_METHOD_DEFINITION(void, onTouchUpEvent)(JNIEnv *, jclass, float x, float y) {
    game->onTouchUpEvent(x, y);
}

} // namespace GameEngineJNI
} // namespace age