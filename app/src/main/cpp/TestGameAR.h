#pragma once

#include <android_game_engine/GameAR.h>

#include <memory>

#define JAVA_ACTIVITY GameActivityAR
#include <android_game_engine/GameEngineJNI.h>

namespace age {

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
                                                  int windowWidth, int windowHeight, int displayRotation,
                                                  jobject j_asset_manager);
JNI_METHOD_DECLARATION(void, onJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y);
JNI_METHOD_DECLARATION(void, onResetJNI)(JNIEnv *env, jobject gameActivity);
}

class GameObject;
class Vehicle;

class TestGameAR : public GameAR {
public:
    TestGameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);

    void onCreate() override;

    void onJoystickInput(float x, float y);

    void onReset();

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    std::shared_ptr<Vehicle> atv = nullptr;
    std::shared_ptr<Vehicle> atvCache = nullptr;
};

} // namespace age