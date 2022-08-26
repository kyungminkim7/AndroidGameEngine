#pragma once

#include <android_game_engine/GameAR.h>

#include <memory>

#include <android_game_engine/GameEngine.h>

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject activity,
                                                  int width, int height, int displayRotation);
JNI_METHOD_DECLARATION(void, onJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y);
JNI_METHOD_DECLARATION(void, onResetJNI)(JNIEnv *env, jobject gameActivity);
}

namespace age {

class GameObject;
class Vehicle;

class GameActivityAR : public GameAR {
public:
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