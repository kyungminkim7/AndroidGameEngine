#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <jni.h>

#include "CameraChase.h"
#include "CameraFPV.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "PhysicsEngine.h"
#include "ShaderProgram.h"
#include "ShadowMap.h"
#include "Skybox.h"
#include "UniformBuffer.h"

namespace age {

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
    Game(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);
    virtual ~Game();

    virtual void onCreate();
    virtual void onStart();
    virtual void onResume();
    virtual void onPause();
    virtual void onStop();
    virtual void onDestroy();

    virtual void onWindowChanged(int width, int height, int displayRotation);

    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    virtual void render();

    virtual bool onTouchDownEvent(float x, float y);
    virtual bool onTouchMoveEvent(float x, float y);
    virtual bool onTouchUpEvent(float x, float y);
    
    void enablePhysicsDebugDrawer(bool enable);

protected:
    JNIEnv* getJNIEnv();
    jobject getJavaApplicationContext();
    jobject getJavaActivityObject();

    void setGravity(const glm::vec3 &gravity);

    void setSkybox(std::unique_ptr<Skybox> skybox);
    
    void addToWorldList(std::shared_ptr<GameObject> gameObject);

    void bindToProjectionViewUBO(ShaderProgram *shaderProgram);
    void bindToLightSpaceUBO(ShaderProgram *shaderProgram);

    ///
    /// Registers a GameObject with the physics engine. This is automatically called for game
    /// objects that are registered to the world list through Game::addToWorldList(). This must be
    /// called or any objects that have not been registered to the world list but want to have
    /// physics along with GameObject::updateFromPhysics() after every Game::onUpdate().
    /// \param gameObject
    ///
    void registerPhysics(GameObject *gameObject);

    void unregisterPhysics(GameObject *gameObject);
    
    virtual void onGameObjectTouched(GameObject *gameObject, const glm::vec3 &touchPoint,
                                     const glm::vec3 &touchDirection, const glm::vec3 &touchNormal);

    virtual void updateUBOs();
    void renderShadowMapSetup();
    void renderShadowMap();
    void renderWorldSetup();
    void renderWorld();

    void bindShadowMap(ShaderProgram *shaderProgram);

    CameraType* getCam();
    LightDirectional* getDirectionalLight();

private:
    void raycastTouch(const glm::vec2 &windowTouchPosition, float length);
    Ray getTouchRay(const glm::vec2 &windowTouchPosition);

    JavaVM *javaVM;
    jobject javaApplicationContext;
    jobject javaActivityObject;

    ShaderProgram shadowMapShader;
    ShaderProgram defaultShader;
    ShaderProgram skyboxShader;
    ShaderProgram physicsDebugShader;

    UniformBuffer projectionViewUbo;
    UniformBuffer lightSpaceUbo;

    int shadowMapTextureUnit; // Shadow map is placed as the last texture unit to deconflict with game object material textures
    
    std::unique_ptr<Skybox> skybox = nullptr;
    std::unique_ptr<CameraType> cam = nullptr;
    std::unique_ptr<LightDirectional> directionalLight = nullptr;
    std::unique_ptr<ShadowMap> shadowMap = nullptr;
    std::vector<std::shared_ptr<GameObject>> worldList;
    
    std::unique_ptr<PhysicsEngine> physics;
    bool drawDebugPhysics;
};

inline CameraType* Game::getCam() {return this->cam.get();}
inline LightDirectional* Game::getDirectionalLight() {return this->directionalLight.get();}

} // namespace age