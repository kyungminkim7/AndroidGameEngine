#include "MobileControlStation.h"

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
(JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
 int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, displayRotation, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<MobileControlStation>(env, gameApplicationContext, gameActivity));
}

JNI_METHOD_DEFINITION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<MobileControlStation*>(GameEngineJNI::getGame())->onLeftJoystickInputJNI(x, y);
}

JNI_METHOD_DEFINITION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<MobileControlStation*>(GameEngineJNI::getGame())->onRightJoystickInputJNI(x, y);
}

JNI_METHOD_DEFINITION(void, onResetJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<MobileControlStation*>(GameEngineJNI::getGame())->onReset();
}

MobileControlStation::MobileControlStation(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject) :
    GameTemplate(env, javaApplicationContext, javaActivityObject) {}

void MobileControlStation::onWindowChanged(int width, int height, int displayRotation) {
    GameTemplate::onWindowChanged(width, height, displayRotation);
}

void MobileControlStation::onUpdate(std::chrono::duration<float> updateDuration) {
    GameTemplate::onUpdate(updateDuration);
}

void MobileControlStation::render() {
}

void MobileControlStation::onLeftJoystickInputJNI(float x, float y) { }
void MobileControlStation::onRightJoystickInputJNI(float x, float y) { }
void MobileControlStation::onReset() {}

} // namespace age