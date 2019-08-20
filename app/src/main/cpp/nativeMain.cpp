#include <android_native_app_glue.h>

#include <android_game_engine/GameEngineNative.h>

#include "nativeTestGame.h"

void android_main(struct android_app *app) {
    age::GameEngineNative::run<age::JNITestGame>(app);
}