#pragma once

#include <android_game_engine/Game.h>

namespace age {

class Quadcopter;

class JNITestGame : public Game {
public:
    void init() override;
    void loadWorld() override;

    void onUpdate(std::chrono::duration<float> updateDuration) override;

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    std::shared_ptr<Quadcopter> uav;
};

} // namespace age