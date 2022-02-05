#pragma once

#include <android_game_engine/Game.h>

#include <chrono>
#include <memory>

#include <network/Node.h>

#include <android_game_engine/GameEngine.h>
#include <android_game_engine/ShaderProgram.h>
#include "ImageMsgDisplay.h"

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject activity,
                                                  int width, int height, int displayRotation);
JNI_METHOD_DECLARATION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity,
    float x, float y);
JNI_METHOD_DECLARATION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity,
    float x, float y);
JNI_METHOD_DECLARATION(void, onResetJNI)(JNIEnv *env, jobject gameActivity);
}

namespace age {

class MobileControlStation : public Game {
public:
    MobileControlStation();

    void onWindowChanged(int width, int height, int displayRotation) override;

    void onUpdate(std::chrono::duration<float> updateDuration) override;
    void render() override;

    void onLeftJoystickInput(float x, float y);
    void onRightJoystickInput(float x, float y);

    void onReset();

private:
    ShaderProgram imageMsgDisplayShader;

    ntwk::Node ntwkNode;
    ImageMsgDisplay imgMsgDisplay;
};

} // namespace age
