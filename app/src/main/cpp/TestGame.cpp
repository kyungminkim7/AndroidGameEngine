#include "TestGame.h"

#include <array>
#include <functional>
#include <memory>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/trigonometric.hpp>

#include <android_game_engine/Box.h>
#include <android_game_engine/ManagerWindowing.h>

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
    (JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
     int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, displayRotation, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<TestGame>(env, gameApplicationContext, gameActivity));
}

JNI_METHOD_DEFINITION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onLeftJoystickInputJNI(x, y);
}

JNI_METHOD_DEFINITION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onRightJoystickInputJNI(x, y);
}

JNI_METHOD_DEFINITION(void, onResetJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onReset();
}

TestGame::TestGame(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject)
    : Game(env, javaApplicationContext, javaActivityObject) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void TestGame::onCreate() {
    Game::onCreate();

//    this->enablePhysicsDebugDrawer(true);
    this->getDirectionalLight()->setLookAtDirection({1.0f, 1.0f, -3.0f});

    this->getCam()->setPosition({-0.5f, 0.0f, 1.0f});
    this->getCam()->setLookAtPoint({0.5f, 0.0f, 0.7f});

    const auto scale = 50.0f;
    std::shared_ptr<Box> floor(new Box({Texture2D("images/wood.png")},
                                       {Texture2D(glm::vec3(1.0f))},
                                       glm::vec2(scale)));

    floor->setLabel("Floor");
    floor->setScale(glm::vec3{scale, scale, 0.2f});
    floor->setPosition(glm::vec3(0.0f));
    floor->setSpecularExponent(32.0f);
    floor->setFriction(1.0f);
    this->addToWorldList(floor);

    this->box = std::shared_ptr<Box>(new Box({Texture2D(glm::vec3(1.0f, 0.0f, 0.0f))},
                                             {Texture2D(glm::vec3(1.0f))}));
    this->box->setScale(glm::vec3(0.1f));
    this->box->setMass(1.0f);
    this->box->setPosition({1.0f, 0.0f, 1.0f});
    this->box->applyCentralForce({0.0f, 0.0f, -1.0f});
    this->addToWorldList(this->box);
}

void TestGame::onLeftJoystickInputJNI(float x, float y) { this->getCam()->onMove({x, y}); }

void TestGame::onRightJoystickInputJNI(float x, float y) { this->getCam()->onRotate({x, y}); }

void TestGame::onReset() { }

void TestGame::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                   const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    this->box->setPosition(touchPoint + glm::vec3(0.0f, 0.0f, 0.5f));
    this->box->setOrientation(glm::mat3(1.0f));
    auto lookAtDirection = this->getCam()->getLookAtDirection();
    lookAtDirection.z = 0.025f;
    this->box->setLookAtDirection(lookAtDirection);
    this->box->clearForces();
    this->box->applyCentralForce({0.0f, 0.0f, -1.0f});
}

} // namespace age
