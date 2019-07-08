#include "TestGame.h"

#include <array>
#include <functional>

#include <glm/trigonometric.hpp>

#include <android_game_engine/Box.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Quadcopter.h>
#include <android_game_engine/Window.h>

namespace age {

void TestGame::init() {
    Game::init();
    this->enablePhysicsDebugDrawer(true);
    
    this->setupGui();
}

void TestGame::setupGui() {
    using namespace std::placeholders;
    
    // Set up joysticks
    const glm::vec2 positionOffset(150.0f, 450.0f);
    const glm::vec2 dimensions(250.0f, 250.0f);
    const glm::vec4 joystickColor(0.0f, 0.6f, 0.7f, 0.8f);
    const glm::vec4 joystickHandleColor(0.0f, 0.7f, 0.9f, 0.8f);
    
    this->moveJoystick = Joystick::New(this->getGui());
    this->moveJoystick->setGeometry(positionOffset, dimensions);
    this->moveJoystick->setColor(joystickColor);
    this->moveJoystick->setHandleColor(joystickHandleColor);
    
    this->rotateJoystick = Joystick::New(this->getGui());
    this->rotateJoystick->setGeometry({ManagerWindowing::getWindowWidth() - positionOffset.x - dimensions.x,
                                       positionOffset.y},
                                      dimensions);
    this->rotateJoystick->setColor(joystickColor);
    this->rotateJoystick->setHandleColor(joystickHandleColor);
    
    auto moveCam = std::bind(&CameraFPV::onMove, this->getCam(), _1);
    this->moveJoystick->registerOnTouchDownCallback(moveCam);
    this->moveJoystick->registerOnTouchMoveCallback(moveCam);
    this->moveJoystick->registerOnTouchUpCallback(std::bind(&CameraFPV::onMove, this->getCam(),
                                                            glm::vec2(0.0f)));

    auto rotateCam = std::bind(&CameraFPV::onRotate, this->getCam(), _1);
    this->rotateJoystick->registerOnTouchDownCallback(rotateCam);
    this->rotateJoystick->registerOnTouchMoveCallback(rotateCam);
    this->rotateJoystick->registerOnTouchUpCallback(std::bind(&CameraFPV::onRotate, this->getCam(),
                                                              glm::vec2(0.0f)));
}

void TestGame::loadWorld() {
    this->getCam()->setPosition({-15.0f, 5.0f, 10.0f});
    this->getCam()->setLookAtPoint({0.0f, 0.0f, 2.0f});

    {
        std::shared_ptr<Box> box1(new Box({Texture2D(glm::vec3(1.0f, 1.0f, 0.0f))},
                                          {Texture2D(glm::vec3(1.0f))}));
        box1->setLabel("Box1");
        box1->setPosition({0.0f, 1.0f, 3.0f});
        box1->setScale({1.0f, 2.0f, 3.0f});
        box1->setMass(1.0f);
        this->addToWorldList(box1);
    }

    {
        const auto scale = 100.0f;
        std::shared_ptr<Box> floor(new Box({Texture2D("images/wood.png")},
                                           {Texture2D(glm::vec3(1.0f))},
                                           glm::vec2(scale)));
        floor->setLabel("Floor");
        floor->setScale(glm::vec3{scale, scale, 0.1f});
        floor->setPosition({0.0f, 0.0f, -0.5f});
        floor->setSpecularExponent(32.0f);
        this->addToWorldList(floor);
    }
    
    {
        using namespace std::placeholders;

        // Create UAV
        this->uav = std::make_shared<Quadcopter>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds");
        this->uav->setLabel("UAV");
        this->uav->setScale(glm::vec3(50.0f));
        this->uav->setPosition({0.0f, 0.0f, 30.0f});
        this->uav->setMass(1.0f);

        // Connect UAV to joystick controls
        this->uav->setMaxMotorThrust(10.0f);
//        this->getCam()->setChaseObject(uav.get(), {-15.0f, 0.0f, 5.0f});

//        auto leftThrottle = std::bind(&Quadcopter::onThrottleInput, this->uav.get(), _1);
//        this->moveJoystick->registerOnTouchDownCallback(leftThrottle);
//        this->moveJoystick->registerOnTouchMoveCallback(leftThrottle);
//        this->moveJoystick->registerOnTouchUpCallback(std::bind(&Quadcopter::onThrottleInput, this->uav.get(), glm::vec2(0.0f)));
//
//        auto rightThrottle = [ptr=this->uav.get()](const auto& input){
//            glm::vec3 force{input.y * 20, 0.0f, 0.0f};
//            ptr->applyCentralForce(ptr->getOrientation() * force);
//        };
//        this->rotateJoystick->registerOnTouchDownCallback(rightThrottle);
//        this->rotateJoystick->registerOnTouchMoveCallback(rightThrottle);
//        this->rotateJoystick->registerOnTouchUpCallback(std::bind(&Quadcopter::onControlInput, this->uav.get(),
//                                                                  glm::vec2(0.0f)));

        this->addToWorldList(this->uav);

    }
}

void TestGame::onUpdate(std::chrono::duration<float> updateDuration) {
    Game::onUpdate(updateDuration);
}

void TestGame::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                   const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    gameObject->applyCentralForce(touchDirection * 400.0f);
}

} // namespace age
