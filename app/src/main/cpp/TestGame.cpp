#include "TestGame.h"

#include <array>
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
    this->enablePhysicsDebugDrawer(true);
    
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
//    const std::string skyboxDir = "skyboxes/early_morning/";
//    std::array<std::string, 6> skyboxImages {
//        skyboxDir + "front.jpg",
//        skyboxDir + "back.jpg",
//        skyboxDir + "right.jpg",
//        skyboxDir + "left.jpg",
//        skyboxDir + "top.jpg",
//        skyboxDir + "bottom.jpg"
//    };
//    this->setSkybox(std::make_unique<Skybox>(skyboxImages));
    
    this->getCam()->setPosition({-10.0f, 0.0f, 2.0f});
    this->getCam()->setLookAtPoint({0.0f, 0.0f, 2.0f});

//    std::unique_ptr<Box> box1(new Box({Texture2D(glm::vec3(1.0f, 1.0f, 0.0f))},
//                                      {Texture2D(glm::vec3(1.0f))}));
//    box1->setPosition({0.0f, 0.5f, 5.0f});
//    box1->setScale({1.0f, 2.0f, 3.0f});
//    box1->setMass(1.0f);
//    this->addToWorldList(std::move(box1));
//
//    std::unique_ptr<Box> box2(new Box({Texture2D(glm::vec3(0.0f, 1.0f, 1.0f))},
//                                      {Texture2D(glm::vec3(1.0f))}));
//    box2->setPosition({0.0f, -0.5f, 5.0f});
//    box2->setMass(1.0f);
//    this->addToWorldList(std::move(box2));
    
    std::unique_ptr<Quad> quad(new Quad({Texture2D("images/wood.png")},
                                        {Texture2D(glm::vec3(1.0f))},
                                        glm::vec2(100.0f)));

    quad->setScale(glm::vec3{100.0f});
    quad->setPosition({0.0f, 0.0f, -0.5f});
    quad->setSpecularExponent(32.0f);

//    this->addToWorldList(std::move(box1));
//    this->addToWorldList(std::move(box2));
    this->addToWorldList(std::move(quad));
    
    {
        auto start = std::chrono::system_clock::now();

        auto uav = std::make_unique<GameObject>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds");
//        auto uav = std::make_unique<GameObject>("models/C-17A_3DS/C-17A_DE.3ds");
//        auto uav = std::make_unique<GameObject>("models/mq9/mq9.3ds");

        uav->setScale(glm::vec3(10.0f));
        uav->setPosition({0.0f, 0.0f, 30.0f});
        uav->setMass(10.0f);

        this->addToWorldList(std::move(uav));

        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        Log::info("1st load time: " + std::to_string(time.count()) + " ms");
    }
    
    {
        auto start = std::chrono::system_clock::now();

        auto uav = std::make_unique<GameObject>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds");
//        auto uav = std::make_unique<GameObject>("models/C-17A_3DS/C-17A_DE.3ds");
        uav->setScale(glm::vec3(5.0f));
        uav->setPosition({1.0f, 1.0f, 1.0f});
        uav->setMass(10.0f);

        this->addToWorldList(std::move(uav));

        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        Log::info("2nd load time: " + std::to_string(time.count()) + " ms");
    }

    {
        auto start = std::chrono::system_clock::now();

        auto uav = std::make_unique<GameObject>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds");
//        auto uav = std::make_unique<GameObject>("models/C-17A_3DS/C-17A_DE.3ds");
        uav->setScale(glm::vec3(15.0f));
        uav->setPosition({1.0f, -1.0f, 1.0f});
        uav->rotate(glm::radians(30.0f), {0.0f, 0.0f, 1.0f});
        uav->setMass(10.0f);

        this->addToWorldList(std::move(uav));

        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
        Log::info("3rd load time: " + std::to_string(time.count()) + " ms");
    }
}

void TestGame::onUpdate(std::chrono::duration<float> updateDuration) {
    Game::onUpdate(updateDuration);
    
//    this->uav->rotate(glm::radians(20.0f) * updateDuration.count(), {1.0f, 1.0f, 1.0f});
}

} // namespace age