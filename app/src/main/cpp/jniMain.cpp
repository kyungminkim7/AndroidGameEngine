#include <android_game_engine/GameEngineJNI.h>

#include "jniTestGame.h"

extern "C" {

JNI_METHOD_DECLARATION(void, init)
(JNIEnv *env, jclass, int windowWidth, int windowHeight, jobject j_asset_manager) {
    age::GameEngineJNI::init(env, windowWidth, windowHeight, j_asset_manager);
    age::GameEngineJNI::loadGame(std::make_unique<age::JNITestGame>());
}

}