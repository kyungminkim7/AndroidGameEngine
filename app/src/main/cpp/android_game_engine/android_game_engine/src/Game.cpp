#include <android_game_engine/Game.h>

#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/ManagerWindowing.h>

namespace age {

Game::Game() : defaultShader("shaders/default.vert", "shaders/default.frag"),
                widgetShader("shaders/widget.vert", "shaders/widget.frag") {}

void Game::init() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    
    // Setup gui
    auto widgetProjection = glm::ortho(0.0f, static_cast<float>(ManagerWindowing::getWindowWidth()),
                                       0.0f, static_cast<float>(ManagerWindowing::getWindowHeight()));
    
    this->widgetShader.use();
    this->widgetShader.setUniform("projection", widgetProjection);
    
    this->gui = Widget::New();
    this->gui->setGeometry({0.0f, 0.0f},
                           {ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight()});

    // Setup cam
    this->cam = std::make_unique<CameraFPV>(45.0f,
                                            static_cast<float>(ManagerWindowing::getWindowWidth()) / ManagerWindowing::getWindowHeight(),
                                            0.1f, 1000.0f);
}

void Game::loadWorld() {}

void Game::onUpdate(std::chrono::duration<float> updateDuration) {
    cam->onUpdate(updateDuration);
}

void Game::render() {
//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    // Render game objects
//    glEnable(GL_DEPTH_TEST);
//    glDisable(GL_BLEND);
//    this->defaultShader.use();
//
//    this->defaultShader.setUniform("projection", this->cam->getProjectionMatrix());
//    this->defaultShader.setUniform("view", this->cam->getViewMatrix());
//
//    this->obj->render(&this->defaultShader);
//
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    this->defaultShader.setVertexAttribPointer("aPos", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//    this->defaultShader.setVertexAttribPointer("aTexCoord", 2, GL_FLOAT, GL_FALSE,
//            5 * sizeof(float), (void*)(3 * sizeof(float)));
//    this->defaultShader.enableVertexAttribArray("aPos");
//    this->defaultShader.enableVertexAttribArray("aTexCoord");
//
//    this->defaultShader.setUniform("texture0", 0);
//    this->defaultShader.setUniform("texture1", 1);
//
//    glActiveTexture(GL_TEXTURE0);
//    this->texture0->bind();
//
//    glActiveTexture(GL_TEXTURE1);
//    this->texture1->bind();
//
//    glDrawArrays(GL_TRIANGLES, 0, 36);
//
//    // Render GUI widgets
//    glDisable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    this->widgetShader.use();
//    this->gui->render(&this->widgetShader);
}

bool Game::onTouchDownEvent(const age::TouchEvent &event) {
    this->gui->onTouchDownEvent(event);
    return true;
}

bool Game::onTouchMoveEvent(const age::TouchEvent &event) {
    this->gui->onTouchMoveEvent(event);
    return true;
}

bool Game::onTouchUpEvent(const age::TouchEvent &event) {
    this->gui->onTouchUpEvent(event);
    return true;
}

} // namespace age