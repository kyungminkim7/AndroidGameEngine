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
    
private:
    void setupGui();
    
    std::shared_ptr<Joystick> moveJoystick;
    std::shared_ptr<Joystick> rotateJoystick;
};

} // namespace age