#pragma once

#include <android_game_engine/Game.h>

#include <memory>

#include <android_game_engine/JNIInterface.h>

extern "C" {
JNI_METHOD_DECLARATION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject activity,
                                                  int width, int height, int displayRotation);
JNI_METHOD_DECLARATION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject activity,
                                                     float x, float y);
JNI_METHOD_DECLARATION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject activity,
                                                      float x, float y);
JNI_METHOD_DECLARATION(void, onResetJNI)(JNIEnv *env, jobject activity);
}

namespace age {

class Box;

class TestGame : public Game {
public:
    void onCreate() override;

    void onLeftJoystickInput(float x, float y);
    void onRightJoystickInput(float x, float y);

    void onReset();

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    void setRandomBoxPositions();

    std::vector<std::shared_ptr<Box>> boxes;
};

} // namespace age