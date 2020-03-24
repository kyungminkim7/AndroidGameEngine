#include <android_game_engine/Game.h>

#include <GLES3/gl32.h>
#include <glm/gtc/matrix_transform.hpp>

#include <android_game_engine/GameObject.h>
#include <android_game_engine/Exception.h>
#include <android_game_engine/LightDirectional.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/PhysicsEngine.h>
#include <android_game_engine/ShadowMap.h>
#include <android_game_engine/Skybox.h>

namespace age {

Game::Game(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject) :
               shadowMapShader("shaders/ShadowMap.vert", "shaders/ShadowMap.frag"),
               defaultShader("shaders/Default.vert", "shaders/Default.frag"),
               skyboxShader("shaders/Skybox.vert", "shaders/Skybox.frag"),
               physicsDebugShader("shaders/PhysicsDebug.vert", "shaders/PhysicsDebug.frag"),
               projectionViewUbo("ProjectionViewUB", sizeof(glm::mat4)),
               lightSpaceUbo("LightSpaceUB", sizeof(glm::mat4)),
               skybox(nullptr), cam(nullptr), directionalLight(nullptr), shadowMap(nullptr),
               physics(new PhysicsEngine(&this->physicsDebugShader)),
               drawDebugPhysics(false) {
    // Obtain and save JNI environment variables
    if (env->GetJavaVM(&this->javaVM) != JNI_OK) throw JNIError("Failed to obtain Java VM.");
    this->javaApplicationContext = env->NewGlobalRef(javaApplicationContext);
    this->javaActivityObject = env->NewGlobalRef(javaActivityObject);

    // Link shaders to necessary UBOs
    this->defaultShader.setUniformBlockBinding(this->projectionViewUbo);
    this->physicsDebugShader.setUniformBlockBinding(this->projectionViewUbo);

    this->defaultShader.setUniformBlockBinding(this->lightSpaceUbo);
    this->shadowMapShader.setUniformBlockBinding(this->lightSpaceUbo);

    // Shadow depth map texture
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &this->shadowMapTextureUnit);
    this->shadowMapTextureUnit -= 1;

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

Game::~Game() {
    auto env = this->getJNIEnv();
    env->DeleteGlobalRef(this->javaActivityObject);
    env->DeleteGlobalRef(this->javaApplicationContext);
};

Game::Game(age::Game &&) = default;
Game& Game::operator=(age::Game &&) = default;

JNIEnv* Game::getJNIEnv() {
    JNIEnv *env;
    if (javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        throw JNIError("Failed to obtain JNIEnv from javaVM");
    }
    return env;
}

jobject Game::getJavaApplicationContext() {return this->javaApplicationContext;}
jobject Game::getJavaActivityObject() {return this->javaActivityObject;}

void Game::onCreate() {
    // Setup cam
    this->cam = std::make_unique<CameraType>(45.0f,
                                             static_cast<float>(ManagerWindowing::getWindowWidth()) / ManagerWindowing::getWindowHeight(),
                                             0.1f, 500.0f);

    // Setup light and shadows
    const auto lightLimit = 10.0f;
    this->directionalLight = std::make_unique<LightDirectional>(glm::vec3(0.2f), glm::vec3(1.0f), glm::vec3(0.8f),
                                                                -lightLimit, lightLimit, -lightLimit, lightLimit,
                                                                0.1f, lightLimit * 2.0f);
    this->directionalLight->setPosition({0.0f, 0.0f, lightLimit * 1.5f});
    this->directionalLight->setNormalDirection({1.0f, 1.0f, 1.0f});
    this->directionalLight->setLookAtPoint({0.0f, 0.0f, 0.0f});

    const auto shadowMapDimension = 2048u;
    this->shadowMap = std::make_unique<ShadowMap>(shadowMapDimension, shadowMapDimension);
}

void Game::onStart() {}
void Game::onResume() {}
void Game::onPause() {}
void Game::onStop() {}
void Game::onDestroy() {}

void Game::onWindowChanged(int width, int height, int displayRotation) {
    this->cam->setAspectRatioWidthToHeight(static_cast<float>(width) / height);
}

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
    this->updateUBOs();

    this->renderShadowMapSetup();
    this->renderShadowMap();

    this->renderWorldSetup();
    this->renderWorld();
}

void Game::updateUBOs() {
    const auto projectionView = this->cam->getProjectionMatrix() * this->cam->getViewMatrix();
    this->projectionViewUbo.bufferSubData(0, sizeof(glm::mat4), glm::value_ptr(projectionView));

    const auto lightSpace = this->directionalLight->getProjectionMatrix() *
            this->directionalLight->getViewMatrix();
    this->lightSpaceUbo.bufferSubData(0, sizeof(glm::mat4), glm::value_ptr(lightSpace));
}

void Game::renderShadowMapSetup() {
    glViewport(0, 0, this->shadowMap->getWidth(), this->shadowMap->getHeight());
    this->shadowMap->bindFramebuffer();

    glCullFace(GL_FRONT);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Game::renderShadowMap() {
    this->shadowMapShader.use();
    for (auto &gameObject : this->worldList) {
        gameObject->render(&this->shadowMapShader);
    }
}

void Game::renderWorldSetup() {
    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glCullFace(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Game::renderWorld() {
    this->defaultShader.use();
    this->defaultShader.setUniform("viewPosition", this->cam->getPosition());

    // Set shadow properties
    this->bindShadowMap(&this->defaultShader);
    this->directionalLight->render(&this->defaultShader);

    for (auto &gameObject : this->worldList) {
        gameObject->render(&this->defaultShader);
    }

    // Render physics debugging attributes
    if (this->drawDebugPhysics) {
        this->physicsDebugShader.use();
        this->physics->renderDebug();
    }

    // Render skybox
    if (this->skybox != nullptr) {
        glDepthFunc(GL_LEQUAL);
        auto view = this->cam->getViewMatrix();
        view[3] = glm::vec4(0.0f);
        this->skyboxShader.use();
        this->skyboxShader.setUniform("projection_view", this->cam->getProjectionMatrix() * view);
        this->skybox->render(&this->defaultShader);
        glDepthFunc(GL_LESS);
    }
}

void Game::bindShadowMap(age::ShaderProgram *shaderProgram) {
    glActiveTexture(GL_TEXTURE0 + this->shadowMapTextureUnit);
    this->shadowMap->bindDepthMap();
    shaderProgram->setUniform("shadowMap", this->shadowMapTextureUnit);
}

bool Game::onTouchDownEvent(float x, float y) {
    this->raycastTouch({x, y}, 1000.0f);
    return true;
}

bool Game::onTouchMoveEvent(float x, float y) {return true;}
bool Game::onTouchUpEvent(float x, float y) {return true;}

void Game::enablePhysicsDebugDrawer(bool enable) {this->drawDebugPhysics = enable;}

void Game::setGravity(const glm::vec3 &gravity) {this->physics->setGravity(gravity);}

void Game::setSkybox(std::unique_ptr<age::Skybox> skybox) {this->skybox = std::move(skybox);}

void Game::addToWorldList(std::shared_ptr<age::GameObject> gameObject) {
    this->registerPhysics(gameObject.get());

    this->worldList.push_back(std::move(gameObject));
}

void Game::clearWorldList() {
    for (auto& gameObject : this->worldList) {
        this->unregisterPhysics(gameObject.get());
    }

    this->worldList.clear();
}

void Game::bindToProjectionViewUBO(age::ShaderProgram *shaderProgram) {
    shaderProgram->setUniformBlockBinding(this->projectionViewUbo);
}

void Game::bindToLightSpaceUBO(age::ShaderProgram *shaderProgram) {
    shaderProgram->setUniformBlockBinding(this->lightSpaceUbo);
}

void Game::registerPhysics(age::GameObject *gameObject) {
    if (gameObject->getPhysicsBody()) {
        this->physics->addRigidBody(gameObject->getPhysicsBody());
    }
}

void Game::unregisterPhysics(age::GameObject *gameObject) {
    this->physics->removeRigidBody(gameObject->getPhysicsBody());
}

void* Game::getNativePhysicsWorld() { return this->physics->getNativePhysicsWorld(); }

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
