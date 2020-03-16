#pragma once

#include <android_game_engine/GameAR.h>

#define GAME_AR
#include <android_game_engine/GameEngineJNI.h>

namespace age {

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
                                                  int windowWidth, int windowHeight, int displayRotation,
                                                  jobject j_asset_manager);
JNI_METHOD_DECLARATION(void, onRollThrustInputJNI)(JNIEnv *env, jobject gameActivity, float roll, float thrust);
JNI_METHOD_DECLARATION(void, onYawPitchInputJNI)(JNIEnv *env, jobject gameActivity, float yaw, float pitch);
JNI_METHOD_DECLARATION(void, onResetUAVJNI)(JNIEnv *env, jobject gameActivity);
}

class Box;

class TestGameAR : public GameAR {
public:
    TestGameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);

    void onCreate() override;

    void onRollThrustInput(float roll, float thrust);
    void onYawPitchInput(float yaw, float pitch);

    void onResetUAV();

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    std::shared_ptr<Box> uav = nullptr;
    std::shared_ptr<Box> uavCache = nullptr;

//    std::shared_ptr<GameObject> random = nullptr;
};

} // namespace age