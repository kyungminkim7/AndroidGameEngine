#include <android_game_engine/Game.h>

#include <GLES2/gl2.h>
#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/PhysicsEngineBullet.h>
#include <android_game_engine/Widget.h>

namespace age {

Game::Game() : defaultShader("shaders/default.vert", "shaders/default.frag"),
               skyboxShader("shaders/skybox.vert", "shaders/skybox.frag"),
               widgetShader("shaders/widget.vert", "shaders/widget.frag"),
               physics(new PhysicsEngineBullet) {}

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
                                            0.1f, 500.0f);
    
    // Setup light
    this->directionalLight = std::make_unique<DirectionalLight>(glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(1.0f),
            -10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 250.0f);
    this->directionalLight->setLookAtDirection({1.0f, 1.0f, -1.0f});
}

void Game::loadWorld() {}

void Game::onUpdate(std::chrono::duration<float> updateDuration) {
    this->cam->onUpdate(updateDuration);
    
    for (auto& gameObject : this->worldList) {
        gameObject->onUpdate(updateDuration);
    }

    this->physics->onUpdate(updateDuration);
    for (auto& gameObject : this->worldList) {
        gameObject->updateFromPhysics();
    }
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render world
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    auto projection = this->cam->getProjectionMatrix();
    auto view = this->cam->getViewMatrix();
    
    this->defaultShader.use();
    this->defaultShader.setUniform("projection_view", projection * view);
    this->defaultShader.setUniform("viewPosition", this->cam->getPosition());
    
    this->directionalLight->render(&this->defaultShader);

    for (auto& gameObject : this->worldList) {
        gameObject->render(&this->defaultShader);
    }
    
    // Render skybox
    if (this->skybox != nullptr) {
        glDepthFunc(GL_LEQUAL);
        view[3] = glm::vec4(0.0f);
        this->skyboxShader.use();
        this->skyboxShader.setUniform("projection_view", projection * view);
        this->skybox->render(&this->defaultShader);
        glDepthFunc(GL_LESS);
    }

    // Render GUI widgets
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    
    this->widgetShader.use();
    this->gui->render(&this->widgetShader);
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

void Game::setSkybox(std::unique_ptr<age::Skybox> skybox) {
    this->skybox = std::move(skybox);
}

void Game::addToWorldList(std::unique_ptr<age::GameObject> gameObject) {
    if (gameObject->getPhysicsBody()) {
        this->physics->addRigidBody(gameObject->getPhysicsBody());
    }
    
    this->worldList.push_back(std::move(gameObject));
}

} // namespace age