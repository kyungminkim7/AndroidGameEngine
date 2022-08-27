#include <android_game_engine/ManagerWindowing.h>

namespace {

int windowWidth = 0;
int windowHeight = 0;
int displayRotation = 0;

} // namespace

namespace age {
namespace ManagerWindowing {

void init(int width, int height, int rotation) {
    windowWidth = width;
    windowHeight = height;
    displayRotation = rotation;
}

void shutdown() {
    windowWidth = 0;
    windowHeight = 0;
}

int getWindowWidth() {
    return windowWidth;
}

int getWindowHeight() {
    return windowHeight;
}

int getDisplayRotation() {
    return displayRotation;
}

} // namespace ManagerWindowing
} // namespace age