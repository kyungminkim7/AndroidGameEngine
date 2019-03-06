#pragma once

#include <chrono>
#include <memory>

#include "CameraFPV.h"
#include "GameObject.h"
#include "MotionEvent.h"
#include "ShaderProgram.h"

namespace age {

/**
 * Game template class.
 * Users should subclass Game and then run it using GameEngine::run with the derived
 * Game class.
 */
class Game {
public:
    Game();
    virtual ~Game() = default;
    
    virtual void init();
    virtual void loadWorld();
    
    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    virtual void render();
    
    virtual bool onMotionDown(const MotionEvent &event);
    virtual bool onMotionMove(const MotionEvent &event);
    virtual bool onMotionUp(const MotionEvent &event);

private:
    ShaderProgram defaultShader;
    
    std::unique_ptr<CameraFPV> cam;
    
    std::unique_ptr<GameObject> obj;
    unsigned int vbo;
};

} // namespace age