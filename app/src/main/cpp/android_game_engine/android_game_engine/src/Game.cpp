#include <android_game_engine/Game.h>

#include <GLES3/gl32.h>
#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Window.h>

namespace age {

Game::Game() : defaultShader("shaders/Default.vert", "shaders/Default.frag"),
               skyboxShader("shaders/Skybox.vert", "shaders/Skybox.frag"),
               widgetShader("shaders/Widget.vert", "shaders/Widget.frag"),
               physicsDebugShader("shaders/PhysicsDebug.vert", "shaders/PhysicsDebug.frag"),
               physics(new PhysicsEngine(&this->physicsDebugShader)),
               drawDebugPhysics(false) {}

void Game::init() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    
    // Setup gui
    auto widgetProjection = glm::ortho(0.0f, static_cast<float>(ManagerWindowing::getWindowWidth()),
                                       0.0f, static_cast<float>(ManagerWindowing::getWindowHeight()));
    
    this->widgetShader.use();
    this->widgetShader.setUniform("projection", widgetProjection);
    
    this->gui = Window::New();
    this->gui->setGeometry({0.0f, 0.0f},
                           {ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight()});
    this->gui->registerOnTouchDownCallback(std::bind(&Game::raycastTouch, this,
                                           std::placeholders::_1, 1000.0f));
    
    // Setup cam
    this->cam = std::make_unique<CameraType>(45.0f,
                                             static_cast<float>(ManagerWindowing::getWindowWidth()) / ManagerWindowing::getWindowHeight(),
                                             0.1f, 500.0f);
    
    // Setup light
    this->directionalLight = std::make_unique<LightDirectional>(glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.8f),
            -10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 250.0f);
    this->directionalLight->setLookAtDirection({-1.0f, -1.0f, -1.0f});
}

void Game::loadWorld() {}

void Game::onUpdate(std::chrono::duration<float> updateDuration) {
    this->cam->onUpdate(updateDuration);
    
    for (auto &gameObject : this->worldList) {
        gameObject->onUpdate(updateDuration);
    }

    this->physics->onUpdate(updateDuration);
    for (auto &gameObject : this->worldList) {
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
    auto projectionView = projection * view;
    
    this->defaultShader.use();
    this->defaultShader.setUniform("projection_view", projectionView);
    this->defaultShader.setUniform("viewPosition", this->cam->getPosition());
    
    this->directionalLight->render(&this->defaultShader);

    for (auto &gameObject : this->worldList) {
        gameObject->render(&this->defaultShader);
    }
    
    if (this->drawDebugPhysics) {
        this->physicsDebugShader.use();
        this->physicsDebugShader.setUniform("projection_view", projectionView);
        this->physics->renderDebug();
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

void Game::enablePhysicsDebugDrawer(bool enable) {
    this->drawDebugPhysics = enable;
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

void Game::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                               const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {}

Ray Game::getTouchRay(const glm::vec2 &windowTouchPosition) {
    auto invProjectionView = glm::inverse(this->cam->getProjectionMatrix() * this->cam->getViewMatrix());
    
    glm::vec2 ndcPosition(windowTouchPosition.x,
                          ManagerWindowing::getWindowHeight() - windowTouchPosition.y);
    ndcPosition /= glm::vec2(ManagerWindowing::getWindowWidth(),
                             ManagerWindowing::getWindowHeight());
    ndcPosition = (ndcPosition - 0.5f) * 2.0f;
    
    glm::vec4 from(ndcPosition, -1.0f, 1.0f);
    glm::vec4 to(ndcPosition, 0.0f, 1.0f);
    
    from = invProjectionView * from;
    from /= from.w;
    
    to = invProjectionView * to;
    to /= to.w;
    
    return {from, glm::normalize(glm::vec3(to - from))};
}

void Game::raycastTouch(const glm::vec2 &windowTouchPosition, float length) {
    auto ray = this->getTouchRay(windowTouchPosition);
    auto result = this->physics->raycastClosest(ray.origin, ray.origin + ray.direction * length);
    
    if (result.gameObject) {
        this->onGameObjectTouched(result.gameObject, result.hitPoint,
                                  ray.direction, result.hitNormal);
    }
}

} // namespace age
