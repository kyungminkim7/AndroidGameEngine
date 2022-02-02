#include <android_game_engine/ManagerWindowing.h>

#include <mutex>

namespace {

std::mutex windowMutex;
int windowWidth = 0;
int windowHeight = 0;
int displayRotation = 0;

} // namespace

namespace age {
namespace ManagerWindowing {

void init(int width, int height, int rotation) {
    const std::lock_guard<std::mutex> guard(windowMutex);
    windowWidth = width;
    windowHeight = height;
    displayRotation = rotation;
}

void shutdown() {
    const std::lock_guard<std::mutex> guard(windowMutex);
    windowWidth = 0;
    windowHeight = 0;
}

int getWindowWidth() {
    const std::lock_guard<std::mutex> guard(windowMutex);
    return windowWidth;
}

int getWindowHeight() {
    const std::lock_guard<std::mutex> guard(windowMutex);
    return windowHeight;
}

int getDisplayRotation() {
    const std::lock_guard<std::mutex> guard(windowMutex);
    return displayRotation;
}

}
} // namespace age