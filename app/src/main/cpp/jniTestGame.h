#pragma once

#include <android_game_engine/Game.h>

#include <android_game_engine/GameEngineJNI.h>

namespace age {

extern "C" {
JNI_METHOD_DECLARATION(void, init)(JNIEnv *env, jclass, int windowWidth, int windowHeight, jobject j_asset_manager);
JNI_METHOD_DECLARATION(void, onRollThrustInput)(JNIEnv *env, jclass, float roll, float thrust);
JNI_METHOD_DECLARATION(void, onYawPitchInput)(JNIEnv *env, jclass, float yaw, float pitch);
}

class Quadcopter;

class JNITestGame : public Game {
public:
    void init() override;
    void loadWorld() override;

    void onRollThrustInput(float roll, float thrust);
    void onYawPitchInput(float yaw, float pitch);

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    std::shared_ptr<Quadcopter> uav;
};

} // namespace age