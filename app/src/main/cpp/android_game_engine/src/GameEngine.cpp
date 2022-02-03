#include <android_game_engine/GameEngine.h>

#include <chrono>

#include <android_game_engine/Game.h>
#include <android_game_engine/ManagerAssets.h>
#include <android_game_engine/ManagerWindowing.h>

namespace {

std::mutex gameMutex;
std::unique_ptr<age::Game> game = nullptr;

} // namespace

namespace age {
namespace GameEngine {

std::unique_lock<std::mutex> getGameLock() {
    return std::unique_lock<std::mutex>(gameMutex);
}

Game *getGame() {
    return game.get();
}

void onCreate(JNIEnv *env, jobject assetManager) {
    ManagerAssets::init(env, assetManager);
}

void onStart() {
    const std::lock_guard<std::mutex> guard(gameMutex);
    if (game) game->onStart();
}

void onResume() {
    const std::lock_guard<std::mutex> guard(gameMutex);
    if (game) game->onResume();
}

void onPause() {
    const std::lock_guard<std::mutex> guard(gameMutex);
    game->onPause();
}

void onStop() {
    const std::lock_guard<std::mutex> guard(gameMutex);
    game->onStop();
}

void onDestroy() {
    {
        const std::lock_guard<std::mutex> guard(gameMutex);
        game->onDestroy();
        game.reset();
    }

    ManagerAssets::shutdown();
    ManagerWindowing::shutdown();
}

void onSurfaceCreated(int width, int height, int displayRotation, std::unique_ptr<Game> &&g) {
    ManagerWindowing::init(width, height, displayRotation);

    const std::lock_guard<std::mutex> guard(gameMutex);
    game = std::move(g);
    game->onCreate();
    game->onStart();
    game->onResume();
}

void onSurfaceChanged(int width, int height, int displayRotation) {
    ManagerWindowing::init(width, height, displayRotation);

    const std::lock_guard<std::mutex> guard(gameMutex);
    game->onWindowChanged(width, height, displayRotation);
}

void update() {
    static std::chrono::system_clock::time_point lastUpdateTime = std::chrono::system_clock::now();

    auto currentUpdateTime = std::chrono::system_clock::now();
    auto updateDuration = currentUpdateTime - lastUpdateTime;
    lastUpdateTime = currentUpdateTime;

    const std::lock_guard<std::mutex> guard(gameMutex);
    game->onUpdate(updateDuration);
    game->render();
}

void onTouchDownEvent(float x, float y) {
    const std::lock_guard<std::mutex> guard(gameMutex);
    game->onTouchDownEvent(x, y);
}

void onTouchMoveEvent(float x, float y) {
    const std::lock_guard<std::mutex> guard(gameMutex);
    game->onTouchMoveEvent(x, y);
}

void onTouchUpEvent(float x, float y) {
    const std::lock_guard<std::mutex> guard(gameMutex);
    game->onTouchUpEvent(x, y);
}

} // namespace GameEngine
} // namespace age