#include <android_game_engine/Game.h>

#include <GLES3/gl32.h>
#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Window.h>

namespace age {

Game::Game() : shadowMapShader("shaders/ShadowMap.vert", "shaders/ShadowMap.frag"),
               defaultShader("shaders/Default.vert", "shaders/Default.frag"),
               skyboxShader("shaders/Skybox.vert", "shaders/Skybox.frag"),
#ifdef NATIVE_GUI
               widgetShader("shaders/Widget.vert", "shaders/Widget.frag"),
#endif
               physicsDebugShader("shaders/PhysicsDebug.vert", "shaders/PhysicsDebug.frag"),
               physics(new PhysicsEngine(&this->physicsDebugShader)),
               drawDebugPhysics(false) {
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &this->shadowMapTextureUnit);
    this->shadowMapTextureUnit -= 1;
}

void Game::init() {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    
    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());

#ifdef NATIVE_GUI
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
#endif
    
    // Setup cam
    this->cam = std::make_unique<CameraType>(45.0f,
                                             static_cast<float>(ManagerWindowing::getWindowWidth()) / ManagerWindowing::getWindowHeight(),
                                             0.1f, 500.0f);
    
    // Setup light and shadows
    const auto lightLimit = 50.0f;
    this->directionalLight = std::make_unique<LightDirectional>(glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.8f),
            -lightLimit, lightLimit, -lightLimit, lightLimit, 10.0f, 200.0f);
    this->directionalLight->setPosition({25.0f, 10.0f, 25.0f});
    this->directionalLight->setLookAtPoint({-5.0f, -5.0f, 0.0f});

    auto shadowMapDimension = 2048u;
    this->shadowMap = std::make_unique<ShadowMap>(shadowMapDimension, shadowMapDimension);
}

void Game::loadWorld() {}

void Game::onWindowSizeChanged(int width, int height) {}

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
    // Render world scene to shadow map from the light's perspective to calculate depth map
    glViewport(0, 0, this->shadowMap->getWidth(), this->shadowMap->getHeight());
    this->shadowMap->bindFramebuffer();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glCullFace(GL_FRONT);
    glClear(GL_DEPTH_BUFFER_BIT);

    auto lightSpace = this->directionalLight->getProjectionMatrix() *
            this->directionalLight->getViewMatrix();

    this->shadowMapShader.use();
    this->shadowMapShader.setUniform("lightSpace", lightSpace);

    for (auto &gameObject : this->worldList) {
        gameObject->render(&this->shadowMapShader);
    }

    // Render world scene with shadow mapping
    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCullFace(GL_BACK);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto projection = this->cam->getProjectionMatrix();
    auto view = this->cam->getViewMatrix();
    auto projectionView = projection * view;

    this->defaultShader.use();
    this->defaultShader.setUniform("lightSpace", lightSpace);
    this->defaultShader.setUniform("projection_view", projectionView);
    this->defaultShader.setUniform("viewPosition", this->cam->getPosition());

    // Set shadow properties
    glActiveTexture(GL_TEXTURE0 + this->shadowMapTextureUnit);
    this->shadowMap->bindDepthMap();
    this->defaultShader.setUniform("shadowMap", this->shadowMapTextureUnit);

    this->directionalLight->render(&this->defaultShader);

    for (auto &gameObject : this->worldList) {
        gameObject->render(&this->defaultShader);
    }

    // Render physics debugging attributes
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

#ifdef NATIVE_GUI
    // Render GUI widgets
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    
    this->widgetShader.use();
    this->gui->render(&this->widgetShader);
#endif
}

bool Game::onTouchDownEvent(float x, float y) {
    this->raycastTouch({x, y}, 1000.0f);
    return true;
}

bool Game::onTouchMoveEvent(float x, float y) {return true;}
bool Game::onTouchUpEvent(float x, float y) {return true;}

#ifdef NATIVE_GUI
bool Game::onTouchDownEvent(const std::vector<TouchEvent> &events) {
    this->gui->onTouchDownEvent(events);
    return true;
}

bool Game::onTouchMoveEvent(const std::vector<TouchEvent> &events) {
    this->gui->onTouchMoveEvent(events);
    return true;
}

bool Game::onTouchUpEvent(const std::vector<TouchEvent> &events) {
    this->gui->onTouchUpEvent(events);
    return true;
}
#endif

void Game::enablePhysicsDebugDrawer(bool enable) {
    this->drawDebugPhysics = enable;
}

void Game::setSkybox(std::unique_ptr<age::Skybox> skybox) {
    this->skybox = std::move(skybox);
}

void Game::addToWorldList(std::shared_ptr<age::GameObject> gameObject) {
    if (gameObject->getPhysicsBody()) {
        this->physics->addRigidBody(gameObject->getPhysicsBody());
    }

    this->worldList.push_back(std::move(gameObject));
}

void Game::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                               const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {}

void Game::raycastTouch(const glm::vec2 &windowTouchPosition, float length) {
    auto ray = this->getTouchRay(windowTouchPosition);
    auto result = this->physics->raycastClosest(ray.origin, ray.origin + ray.direction * length);

    if (result.gameObject) {
        this->onGameObjectTouched(result.gameObject, result.hitPoint,
                                  ray.direction, result.hitNormal);
    }
}

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

} // namespace age
