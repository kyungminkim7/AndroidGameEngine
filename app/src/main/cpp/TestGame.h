#pragma once

#include <android_game_engine/Game.h>
#include <android_game_engine/Joystick.h>

namespace age {

class TestGame : public Game {
public:
    void init() override;
    void loadWorld() override;

private:
    void setupGui();
    
    std::shared_ptr<Joystick> moveJoystick;
    std::shared_ptr<Joystick> rotateJoystick;
};

} // namespace age