#pragma once

#include <android_game_engine/GameAR.h>

#include <memory>

#define GAME_AR
#include <android_game_engine/GameEngineJNI.h>

namespace age {

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
                                                  int windowWidth, int windowHeight, int displayRotation,
                                                  jobject j_asset_manager);
JNI_METHOD_DECLARATION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y);
JNI_METHOD_DECLARATION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y);
JNI_METHOD_DECLARATION(void, onResetJNI)(JNIEnv *env, jobject gameActivity);
}

class Box;

class TestGameAR : public GameAR {
public:
    TestGameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);

    void onCreate() override;

    void onLeftJoystickInput(float x, float y);
    void onRightJoystickInput(float x, float y);

    void onReset();

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    std::shared_ptr<Box> box = nullptr;
    std::shared_ptr<Box> boxCache = nullptr;
};

} // namespace age