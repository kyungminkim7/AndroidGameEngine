#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include "CameraChase.h"
#include "CameraFPV.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "PhysicsEngine.h"
#include "TouchEvent.h"
#include "ShaderProgram.h"
#include "Skybox.h"

namespace age {

class Window;

using CameraType = CameraFPV;

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
    
    void enablePhysicsDebugDrawer(bool enable);

protected:
    void setSkybox(std::unique_ptr<Skybox> skybox);
    void addToWorldList(std::unique_ptr<GameObject> gameObject);
    
    Window* getGui();
    CameraType* getCam();
    LightDirectional* getDirectionalLight();

private:
    ShaderProgram defaultShader;
    ShaderProgram skyboxShader;
    ShaderProgram widgetShader;
    ShaderProgram physicsDebugShader;
    
    std::shared_ptr<Window> gui = nullptr;
    
    std::unique_ptr<Skybox> skybox = nullptr;
    std::unique_ptr<CameraType> cam = nullptr;
    std::unique_ptr<LightDirectional> directionalLight = nullptr;
    std::vector<std::unique_ptr<GameObject>> worldList;
    
    std::unique_ptr<PhysicsEngine> physics;
    bool drawDebugPhysics;
};

inline Window* Game::getGui() {return this->gui.get();}
inline CameraType* Game::getCam() {return this->cam.get();}
inline LightDirectional* Game::getDirectionalLight() {return this->directionalLight.get();}

} // namespace age