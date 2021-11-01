#pragma once

#include <android_game_engine/Game.h>

#include <memory>

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

class TestGame : public Game {
public:
    TestGame(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);

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