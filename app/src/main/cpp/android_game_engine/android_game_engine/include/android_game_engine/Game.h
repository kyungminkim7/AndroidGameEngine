#pragma once

#include <chrono>
#include <memory>

#include "Camera.h"
#include "GameObject.h"
#include "TouchEvent.h"
#include "ShaderProgram.h"
#include "Widget.h"

namespace age {

/**
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
    
    virtual bool onTouchDownEvent(const TouchEvent &event);
    virtual bool onTouchMoveEvent(const TouchEvent &event);
    virtual bool onTouchUpEvent(const TouchEvent &event);

protected:
    Widget* getGui();
    Camera* getCam();
    
    ShaderProgram* getDefaultShader();
    ShaderProgram* getWidgetShader();

private:
    ShaderProgram defaultShader;
    ShaderProgram widgetShader;
    
    std::shared_ptr<Widget> gui;
    
    std::unique_ptr<Camera> cam;
};

inline Widget* Game::getGui() {return this->gui.get();}
inline Camera* Game::getCam() {return this->cam.get();}
inline ShaderProgram* Game::getDefaultShader() {return &this->defaultShader;}
inline ShaderProgram* Game::getWidgetShader() {return &this->widgetShader;}

} // namespace age