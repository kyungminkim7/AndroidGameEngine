#include "GameActivity.h"

#include <random>

#include <android_game_engine/Box.h>
#include <android_game_engine/Texture2D.h>

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)(JNIEnv *env, jobject activity,
                                                 int width, int height, int rotation) {
    age::GameEngine::onSurfaceCreated(width, height, rotation,
                                      std::make_unique<age::GameActivity>());
}

JNI_METHOD_DEFINITION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity,
        float x, float y) {
    reinterpret_cast<age::GameActivity*>(age::GameEngine::getGame())->onLeftJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity,
        float x, float y) {
    reinterpret_cast<age::GameActivity*>(age::GameEngine::getGame())->onRightJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onResetJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<age::GameActivity*>(age::GameEngine::getGame())->onReset();
}

namespace age {

void GameActivity::onCreate() {
    Game::onCreate();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

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

void GameActivity::onLeftJoystickInput(float x, float y) { this->getCam()->onMove({x, y}); }

void GameActivity::onRightJoystickInput(float x, float y) { this->getCam()->onRotate({x, y}); }

void GameActivity::onReset() {
    for (auto& box : this->boxes) {
        box->clearForces();
        box->setLinearVelocity(glm::vec3(0.0f));
        box->setAngularVelocity(glm::vec3(0.0f));
    }

    this->setRandomBoxPositions();
}

void GameActivity::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                       const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    gameObject->applyCentralForce(touchDirection * 1000.0f);
}

void GameActivity::setRandomBoxPositions() {
    std::mt19937 rand;
    std::uniform_real_distribution<float> xy(-2.0f, 2.0f);
    std::uniform_real_distribution<float> z(0.0f, 3.0f);
    for (auto& box : this->boxes) {
        box->setPosition({1.0f + xy(rand), 0.0f + xy(rand), 1.0f + z(rand)});
        box->applyCentralForce({0.0f, 0.0f, -1.0f});
    }
}

} // namespace age