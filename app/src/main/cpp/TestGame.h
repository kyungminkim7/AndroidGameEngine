#pragma once

#include <android_game_engine/Game.h>
#include <android_game_engine/Joystick.h>

namespace age {

class Box;

class TestGame : public Game {
public:
    void init() override;
    void loadWorld() override;

    void onUpdate(std::chrono::duration<float> updateDuration) override;

protected:
    void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                             const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) override;
    
private:
    void setupGui();
    
    std::shared_ptr<Joystick> moveJoystick;
    std::shared_ptr<Joystick> rotateJoystick;
    
    GameObject *obj;
};

} // namespace age