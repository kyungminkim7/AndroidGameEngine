#include "MobileControlStation.h"

#include <GLES3/gl32.h>

#include <android_game_engine/Log.h>

#include <geometry_msgs/Vector3_generated.h>

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
(JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
 int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, displayRotation, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<MobileControlStation>(env, gameApplicationContext, gameActivity));
}

JNI_METHOD_DEFINITION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<MobileControlStation*>(GameEngineJNI::getGame())->onLeftJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<MobileControlStation*>(GameEngineJNI::getGame())->onRightJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onResetJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<MobileControlStation*>(GameEngineJNI::getGame())->onReset();
}

MobileControlStation::MobileControlStation(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject) :
        GameTemplate(env, javaApplicationContext, javaActivityObject),
        imageMsgDisplayShader("shaders/ImageMsgDisplay.vert", "shaders/ImageMsgDisplay.frag") {
    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());

    const std::string robotIp = "192.168.1.110";
    this->imgSubscriber = this->ntwkNode.subscribe(robotIp, 50000, 1, [this](auto imgMsgBuffer){
        this->imgMsgDisplay.bufferImage(imgMsgBuffer.get());
    });

//    this->imgSubscriber = this->ntwkNode.subscribe(robotIp, 50001, [](auto msgBuffer) {
//        auto vec3 = geometry_msgs::GetVector3(msgBuffer.get());
//        Log::info("(" + std::to_string(vec3->x()) + ", " + std::to_string(vec3->y()) + ", " + std::to_string(vec3->z()) + ")");
//    });
}

void MobileControlStation::onWindowChanged(int width, int height, int displayRotation) {
    GameTemplate::onWindowChanged(width, height, displayRotation);

    glViewport(0, 0, width, height);
}

void MobileControlStation::onUpdate(std::chrono::duration<float> updateDuration) {
    GameTemplate::onUpdate(updateDuration);

    this->ntwkNode.update();
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