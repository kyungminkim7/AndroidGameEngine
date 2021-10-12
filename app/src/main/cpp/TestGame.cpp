#include "TestGame.h"

#include <random>

#include <android_game_engine/Box.h>
#include <android_game_engine/LightDirectional.h>
#include <android_game_engine/Texture2D.h>

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
    (JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
     int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, displayRotation, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<TestGame>(env, gameApplicationContext, gameActivity));
}

JNI_METHOD_DEFINITION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onLeftJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onRightJoystickInput(x, y);
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

    this->enablePhysicsDebugDrawer(true);
    this->getDirectionalLight()->setLookAtDirection({1.0f, 1.0f, -3.0f});

    this->getCam()->setPosition({-3.0f, 0.0f, 1.5f});
    this->getCam()->setLookAtPoint({0.5f, 0.0f, 0.7f});

    // Create floor
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

    // Create boxes
    std::mt19937 rand;
    std::uniform_real_distribution<float> color(0.0f, 1.0f);

    constexpr auto numBoxes = 100u;
    this->boxes.reserve(numBoxes);
    for (auto i = 0u; i < numBoxes; ++i) {
        this->boxes.push_back(std::shared_ptr<Box>(new Box({Texture2D(glm::vec3(color(rand), color(rand), color(rand)))},
                                                           {Texture2D(glm::vec3(1.0f))})));
        this->boxes.back()->setScale(glm::vec3(0.2f));
        this->boxes.back()->setMass(1.0f);
        this->addToWorldList(this->boxes.back());
    }

    this->setRandomBoxPositions();
}

void TestGame::onLeftJoystickInput(float x, float y) { this->getCam()->onMove({x, y}); }

void TestGame::onRightJoystickInput(float x, float y) { this->getCam()->onRotate({x, y}); }

void TestGame::onReset() {
    for (auto& box : this->boxes) {
        box->clearForces();
        box->setLinearVelocity(glm::vec3(0.0f));
        box->setAngularVelocity(glm::vec3(0.0f));
    }

    this->setRandomBoxPositions();
}

void TestGame::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                   const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    gameObject->applyCentralForce(touchDirection * 1000.0f);
}

void TestGame::setRandomBoxPositions() {
    std::mt19937 rand;
    std::uniform_real_distribution<float> xy(-2.0f, 2.0f);
    std::uniform_real_distribution<float> z(0.0f, 3.0f);
    for (auto& box : this->boxes) {
        box->setPosition({1.0f + xy(rand), 0.0f + xy(rand), 1.0f + z(rand)});
        box->applyCentralForce({0.0f, 0.0f, -1.0f});
    }
}

} // namespace age
