#pragma once

#include <android_game_engine/Game.h>
#include <android_game_engine/Joystick.h>
#include <android_game_engine/Texture2D.h>
#include <android_game_engine/Widget.h>

namespace age {

class TestGame : public Game {
public:
    void init() override;
    void loadWorld() override;
    
    void onUpdate(std::chrono::duration<float> updateDuration) override;
    void render() override;

private:
    void setupGui();
    
    std::shared_ptr<Joystick> moveJoystick;
    std::shared_ptr<Joystick> rotateJoystick;
    
    std::unique_ptr<GameObject> obj;
    unsigned int vbo;
    std::unique_ptr<Texture2D> texture0, texture1;
};

} // namespace age