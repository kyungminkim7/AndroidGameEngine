#pragma once

#include <chrono>

#include "ShaderProgram.h"

namespace age {

class Game {
public:
    Game();
    virtual ~Game() = default;
    
    virtual void init();
    
    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    virtual void render();

private:
    ShaderProgram defaultShader;
    
    unsigned int VBO;
};

} // namespace age