#include <android_native_app_glue.h>

#include <android_game_engine/GameEngine.h>

#include "TestGame.h"

void android_main(struct android_app *app) {
    age::GameEngine::run<age::TestGame>(app);
}