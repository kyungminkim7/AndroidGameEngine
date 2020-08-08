#pragma once

#include <android_game_engine/GameTemplate.h>

#include <chrono>
#include <memory>

#include <network/Node.h>
#include <network/TcpSubscriber.h>

#define JAVA_ACTIVITY MobileControlStationActivity
#include <android_game_engine/GameEngineJNI.h>

#include <android_game_engine/ShaderProgram.h>
#include "ImageMsgDisplay.h"

namespace age {

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
                                                  int windowWidth, int windowHeight, int displayRotation,
                                                  jobject j_asset_manager);
JNI_METHOD_DECLARATION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y);
JNI_METHOD_DECLARATION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y);
JNI_METHOD_DECLARATION(void, onResetJNI)(JNIEnv *env, jobject gameActivity);
}

class MobileControlStation : public GameTemplate {
public:
    MobileControlStation(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);

    void onWindowChanged(int width, int height, int displayRotation) override;

    void onUpdate(std::chrono::duration<float> updateDuration) override;
    void render() override;

    void onLeftJoystickInput(float x, float y);
    void onRightJoystickInput(float x, float y);

    void onReset();

private:
    ShaderProgram imageMsgDisplayShader;

    ntwk::Node ntwkNode;
    std::shared_ptr<ntwk::TcpSubscriber<ntwk::Image, ntwk::Compression::Image::JpegPolicy>> imgSubscriber;
    ImageMsgDisplay imgMsgDisplay;
};

} // namespace age
