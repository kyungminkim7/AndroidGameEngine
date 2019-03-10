#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include "CameraFPV.h"
#include "GameObject.h"
#include "TouchEvent.h"
#include "ShaderProgram.h"

namespace age {

class Widget;

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
    CameraFPV* getCam();
    
    ShaderProgram* getDefaultShader();
    ShaderProgram* getWidgetShader();

private:
    ShaderProgram defaultShader;
    ShaderProgram widgetShader;
    
    std::shared_ptr<Widget> gui;
    
    std::unique_ptr<CameraFPV> cam;
    std::vector<std::unique_ptr<GameObject>> worldList;
};

inline Widget* Game::getGui() {return this->gui.get();}
inline CameraFPV* Game::getCam() {return this->cam.get();}
inline ShaderProgram* Game::getDefaultShader() {return &this->defaultShader;}
inline ShaderProgram* Game::getWidgetShader() {return &this->widgetShader;}

} // namespace age