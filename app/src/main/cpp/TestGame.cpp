#include "TestGame.h"

#include <functional>

#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Widget.h>

namespace {
std::vector<float> vertices = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
} // namespace

namespace age {

void TestGame::init() {
    Game::init();
    
    this->setupGui();
}

void TestGame::setupGui() {
    using namespace std::placeholders;
    
    // Set up joysticks
    glm::vec2 positionOffset(100.0f, 450.0f);
    glm::vec2 dimensions(250.0f, 250.0f);
    glm::vec4 joystickColor(0.0f, 0.6f, 0.7f, 0.8f);
    glm::vec4 joystickHandleColor(0.0f, 0.7f, 0.9f, 0.8f);
    
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
    
    this->obj = std::make_unique<GameObject>();
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    this->texture0 = std::make_unique<Texture2D>("images/container.jpg");
    this->texture1 = std::make_unique<Texture2D>("images/awesomeface.png");
}

void TestGame::onUpdate(std::chrono::duration<float> updateDuration) {
    Game::onUpdate(updateDuration);
}

void TestGame::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render game objects
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    auto defaultShader = this->getDefaultShader();
    defaultShader->use();
    
    defaultShader->setUniform("projection", this->getCam()->getProjectionMatrix());
    defaultShader->setUniform("view", this->getCam()->getViewMatrix());
    
    this->obj->render(defaultShader);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    defaultShader->setVertexAttribPointer("aPos", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    defaultShader->setVertexAttribPointer("aTexCoord", 2, GL_FLOAT, GL_FALSE,
                                               5 * sizeof(float), (void*)(3 * sizeof(float)));
    defaultShader->enableVertexAttribArray("aPos");
    defaultShader->enableVertexAttribArray("aTexCoord");
    
    defaultShader->setUniform("texture0", 0);
    defaultShader->setUniform("texture1", 1);
    
    glActiveTexture(GL_TEXTURE0);
    this->texture0->bind();
    
    glActiveTexture(GL_TEXTURE1);
    this->texture1->bind();
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Render GUI widgets
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    this->getWidgetShader()->use();
    this->getGui()->render(this->getWidgetShader());
}

} // namespace age