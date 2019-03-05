#pragma once

#include <chrono>
#include <memory>

#include "Camera.h"
#include "GameObject.h"
#include "ShaderProgram.h"

namespace age {

class Game {
public:
    Game();
    virtual ~Game() = default;
    
    virtual void init();
    virtual void loadWorld();
    
    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    virtual void render();

private:
    ShaderProgram defaultShader;
    
    std::unique_ptr<Camera> cam;
    
    std::unique_ptr<GameObject> obj;
    unsigned int vbo;
};

} // namespace age