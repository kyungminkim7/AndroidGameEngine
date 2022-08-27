#include "MobileControlStation.h"

#include <utility>

#include <GLES3/gl32.h>

#include <network/MsgTypeId.h>

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject activity,
                                                 int width, int height, int displayRotation) {
    age::GameEngine::onSurfaceCreated(width, height, displayRotation,
                                      std::make_unique<age::MobileControlStation>());
}

JNI_METHOD_DEFINITION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity,
                                                    float x, float y) {
    reinterpret_cast<age::MobileControlStation*>(age::GameEngine::getGame())->onLeftJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity,
                                                     float x, float y) {
    reinterpret_cast<age::MobileControlStation*>(age::GameEngine::getGame())->onRightJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onResetJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<age::MobileControlStation*>(age::GameEngine::getGame())->onReset();
}

namespace age {

MobileControlStation::MobileControlStation() : Game(),
    imageMsgDisplayShader("shaders/ImageMsgDisplay.vert",
                          "shaders/ImageMsgDisplay.frag") {
    auto remoteHost = std::make_pair("192.168.1.195", 2048);
    this->ntwkNode.subscribe(remoteHost, ntwk::MsgTypeId::IMAGE_JPEG, [this](auto &&msg){
        this->imgMsgDisplay.bufferImage(msg.get());
    });
}

void MobileControlStation::onWindowChanged(int width, int height, int displayRotation) {
    Game::onWindowChanged(width, height, displayRotation);
    glViewport(0, 0, width, height);
}

void MobileControlStation::onUpdate(std::chrono::duration<float> updateDuration) {
    Game::onUpdate(updateDuration);
    this->ntwkNode.runOnce();
}

void MobileControlStation::render() {
    glClear(GL_COLOR_BUFFER_BIT);

    this->imageMsgDisplayShader.use();
    this->imgMsgDisplay.render(&this->imageMsgDisplayShader);
}

void MobileControlStation::onLeftJoystickInput(float x, float y) { }
void MobileControlStation::onRightJoystickInput(float x, float y) { }
void MobileControlStation::onReset() {}

} // namespace age