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
#include "ShadowMap.h"
#include "Skybox.h"

namespace age {

#ifdef NATIVE_GUI
class Window;
#endif

//using CameraType = CameraFPV;
using CameraType = CameraChase;

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

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

    virtual void onWindowSizeChanged(int width, int height);
    
    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    virtual void render();

    virtual bool onTouchDownEvent(float x, float y);
    virtual bool onTouchMoveEvent(float x, float y);
    virtual bool onTouchUpEvent(float x, float y);

#ifdef NATIVE_GUI
    virtual bool onTouchDownEvent(const std::vector<TouchEvent> &event);
    virtual bool onTouchMoveEvent(const std::vector<TouchEvent> &event);
    virtual bool onTouchUpEvent(const std::vector<TouchEvent> &event);
#endif
    
    void enablePhysicsDebugDrawer(bool enable);

protected:
    void setSkybox(std::unique_ptr<Skybox> skybox);
    
    void addToWorldList(std::shared_ptr<GameObject> gameObject);
    
    virtual void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                                     const glm::vec3 &touchDirection, const glm::vec3 &touchNormal);

#ifdef NATIVE_GUI
    Window* getGui();
#endif

    CameraType* getCam();
    LightDirectional* getDirectionalLight();

private:
    void raycastTouch(const glm::vec2 &windowTouchPosition, float length);
    Ray getTouchRay(const glm::vec2 &windowTouchPosition);

    ShaderProgram shadowMapShader;
    ShaderProgram defaultShader;
    ShaderProgram skyboxShader;
#ifdef NATIVE_GUI
    ShaderProgram widgetShader;
#endif
    ShaderProgram physicsDebugShader;

    int shadowMapTextureUnit; // Shadow map is placed as the last texture unit to deconflict with game object material textures

#ifdef NATIVE_GUI
    std::shared_ptr<Window> gui = nullptr;
#endif
    
    std::unique_ptr<Skybox> skybox = nullptr;
    std::unique_ptr<CameraType> cam = nullptr;
    std::unique_ptr<LightDirectional> directionalLight = nullptr;
    std::unique_ptr<ShadowMap> shadowMap = nullptr;
    std::vector<std::shared_ptr<GameObject>> worldList;
    
    std::unique_ptr<PhysicsEngine> physics;
    bool drawDebugPhysics;
};

#ifdef  NATIVE_GUI
inline Window* Game::getGui() {return this->gui.get();}
#endif

inline CameraType* Game::getCam() {return this->cam.get();}
inline LightDirectional* Game::getDirectionalLight() {return this->directionalLight.get();}

} // namespace age