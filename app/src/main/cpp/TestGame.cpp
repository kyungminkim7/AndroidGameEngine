#include "TestGame.h"

#include <functional>

#include <glm/trigonometric.hpp>

#include <android_game_engine/Box.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Quad.h>
#include <android_game_engine/Widget.h>

namespace age {

void TestGame::init() {
    Game::init();
    
    this->setupGui();
}

void TestGame::setupGui() {
    using namespace std::placeholders;
    
    // Set up joysticks
    const glm::vec2 positionOffset(100.0f, 450.0f);
    const glm::vec2 dimensions(250.0f, 250.0f);
    const glm::vec4 joystickColor(0.0f, 0.6f, 0.7f, 0.8f);
    const glm::vec4 joystickHandleColor(0.0f, 0.7f, 0.9f, 0.8f);
    
    this->moveJoystick = Joystick::New(this->getGui());
    this->moveJoystick->setGeometry(positionOffset, dimensions);
    this->moveJoystick->setColor(joystickColor);
    this->moveJoystick->setHandleColor(joystickHandleColor);
    
    auto moveCam = std::bind(&CameraFPV::onMove, this->getCam(), _1);
    this->moveJoystick->registerOnTouchDownCallback(moveCam);
    this->moveJoystick->registerOnTouchMoveCallback(moveCam);
    this->moveJoystick->registerOnTouchUpCallback(std::bind(&CameraFPV::onMove, this->getCam(),
                                                            glm::vec2(0.0f)));
    
    this->rotateJoystick = Joystick::New(this->getGui());
    this->rotateJoystick->setGeometry({ManagerWindowing::getWindowWidth() - positionOffset.x - dimensions.x,
                                       positionOffset.y},
                                      dimensions);
    this->rotateJoystick->setColor(joystickColor);
    this->rotateJoystick->setHandleColor(joystickHandleColor);
    
    auto rotateCam = std::bind(&CameraFPV::onRotate, this->getCam(), _1);
    this->rotateJoystick->registerOnTouchDownCallback(rotateCam);
    this->rotateJoystick->registerOnTouchMoveCallback(rotateCam);
    this->rotateJoystick->registerOnTouchUpCallback(std::bind(&CameraFPV::onRotate, this->getCam(),
                                                              glm::vec2(0.0f)));
}

void TestGame::loadWorld() {
    this->getCam()->setPosition({-10.0f, 1.0f, 1.0f});
    this->getCam()->setLookAtPoint(glm::vec3(0.0f));

    std::unique_ptr<Box> box1(new Box({"images/container.jpg"}, {"images/white.png"}));
    std::unique_ptr<Box> box2(new Box({"images/awesomeface.png"}, {"images/white.png"}));
    std::unique_ptr<Quad> quad(new Quad({"images/wood.png"}, {"images/white.png"}, glm::vec3(5.0f)));
    
    this->box = box1.get();
    this->box->setScale({2.0f, 3.0f, 5.0f});
    this->box->setPosition({5.0f, -5.0f, 5.0f});
    
    quad->setScale(glm::vec3{5.0f});
    quad->setPosition({0.0f, 0.0f, -0.5f});
//    quad->setSpecularExponent(32.0f);
    
    this->addToWorldList(std::move(box1));
    this->addToWorldList(std::move(box2));
    this->addToWorldList(std::move(quad));
}

void TestGame::onUpdate(std::chrono::duration<float> updateDuration) {
    Game::onUpdate(updateDuration);
    
    this->box->rotate(glm::radians(20.0f) * updateDuration.count(), {1.0f, 1.0f, 1.0f});
}

} // namespace age