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
    const glm::vec2 dimensions(300.0f, 300.0f);
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

//    auto moveCam = std::bind(&CameraFPV::onMove, this->getCam(), _1);
//    this->moveJoystick->registerOnTouchDownCallback(moveCam);
//    this->moveJoystick->registerOnTouchMoveCallback(moveCam);
//    this->moveJoystick->registerOnTouchUpCallback(std::bind(&CameraFPV::onMove, this->getCam(),
//                                                            glm::vec2(0.0f)));
//
//    auto rotateCam = std::bind(&CameraFPV::onRotate, this->getCam(), _1);
//    this->rotateJoystick->registerOnTouchDownCallback(rotateCam);
//    this->rotateJoystick->registerOnTouchMoveCallback(rotateCam);
//    this->rotateJoystick->registerOnTouchUpCallback(std::bind(&CameraFPV::onRotate, this->getCam(),
//                                                              glm::vec2(0.0f)));
}

void TestGame::loadWorld() {
    this->getCam()->setPosition({-10.0f, 5.0f, 7.0f});
    this->getCam()->setLookAtPoint({2.0f, 0.0f, 1.0f});

    {
        auto obj1 = std::make_shared<GameObject>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds");
        obj1->setLabel("obj1");
        obj1->setPosition({4.0f, 3.0f, 5.0f});
        obj1->setScale(glm::vec3(10.0f));
        obj1->setMass(1.0f);
        this->addToWorldList(obj1);
    }

    {
        const auto scale = 100.0f;
        std::shared_ptr<Box> floor(new Box({Texture2D("images/wood.png")},
                                           {Texture2D(glm::vec3(1.0f))},
                                           glm::vec2(scale)));
        floor->setLabel("Floor");
        floor->setScale(glm::vec3{scale, scale, 0.2f});
        floor->setPosition(glm::vec3(0.0f));
        floor->setSpecularExponent(32.0f);
        floor->setFriction(1.0f);
        this->addToWorldList(floor);
    }

    {
        using namespace std::placeholders;

        // Create UAV
        Quadcopter::Parameters params;
        params.mass = 1.0f;
        params.maxRoll = glm::radians(35.0f);
        params.maxPitch = glm::radians(35.0f);
        params.maxRollRate = glm::radians(360.0f);
        params.maxPitchRate = glm::radians(360.0f);
        params.maxYawRate = glm::radians(45.0f);
        params.maxThrust = 15.0f;
        params.controlRates2MotorRotationSpeed = 150.0f;
        params.angle_kp = 2.5f;
        params.angle_ki = 0.0f;
        params.angle_kd = 0.8f;
        params.motorRotationSpeed2Thrust = 2.0E-3f;

        this->uav = std::make_shared<Quadcopter>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds", params);
        this->uav->setLabel("UAV");
        this->uav->setScale({0.363f, 0.363f, 0.053f});
        this->uav->setPosition({0.0f, 0.0f, 3.0f});
        this->uav->setMode(Quadcopter::Mode::ANGLE);

        // Connect UAV to joystick controls
//        this->getCam()->setChaseObject(uav.get(), {-5.0f, 0.0f, 1.0f});

        auto leftThrottle = std::bind(&Quadcopter::onRollThrustInput, this->uav.get(), _1);
        this->moveJoystick->registerOnTouchDownCallback(leftThrottle);
        this->moveJoystick->registerOnTouchMoveCallback(leftThrottle);
        this->moveJoystick->registerOnTouchUpCallback(std::bind(&Quadcopter::onRollThrustInput, this->uav.get(), glm::vec2(0.0f)));

        auto rightThrottle = std::bind(&Quadcopter::onYawPitchInput, this->uav.get(), _1);
        this->rotateJoystick->registerOnTouchDownCallback(rightThrottle);
        this->rotateJoystick->registerOnTouchMoveCallback(rightThrottle);
        this->rotateJoystick->registerOnTouchUpCallback(std::bind(&Quadcopter::onYawPitchInput, this->uav.get(), glm::vec2(0.0f)));

        this->addToWorldList(this->uav);
    }
}

void TestGame::onUpdate(std::chrono::duration<float> updateDuration) {
    Game::onUpdate(updateDuration);
}

void TestGame::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                   const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    gameObject->applyCentralForce(touchDirection * 400.0f);
    this->uav->setOrientation(glm::mat3(1.0f));
    this->uav->setPosition({0.0f, 0.0f, 0.5f});
}

} // namespace age
