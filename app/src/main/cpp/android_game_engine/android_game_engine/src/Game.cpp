#include <android_game_engine/Game.h>

#include <GLES2/gl2.h>

#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerAssets.h>
#include <android_game_engine/ManagerWindowing.h>

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

Game::Game() : defaultShader("shaders/default.vert", "shaders/default.frag"){}

Game::~Game() {
    glDeleteBuffers(1, &this->vbo);
}

void Game::init() {
    glEnable(GL_DEPTH_TEST);
    
    glViewport(0, 0, ManagerWindowing::getWindowWidth(),ManagerWindowing::getWindowHeight());
}

void Game::loadWorld() {
    this->cam = std::make_unique<CameraFPV>(45.0f,
            static_cast<float>(ManagerWindowing::getWindowWidth()) / ManagerWindowing::getWindowHeight(),
            0.1f, 1000.0f);
    this->cam->setPosition({-10.0f, 1.0f, 1.0f})
          .setLookAtPoint(glm::vec3(0.0f));
    
    this->obj = std::make_unique<GameObject>();
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    this->texture0 = std::make_unique<Texture2D>("images/container.jpg");
    this->texture1 = std::make_unique<Texture2D>("images/awesomeface.png");
}

void Game::onUpdate(std::chrono::duration<float> updateDuration) {
    cam->onUpdate(updateDuration);
}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    this->defaultShader.use();
    
    this->defaultShader.setUniform("projection", this->cam->getProjectionMatrix())
            .setUniform("view", this->cam->getViewMatrix());
    
    this->defaultShader.setUniform("model", this->obj->getModelMatrix())
            .setUniform("normal", this->obj->getNormalMatrix());
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    this->defaultShader.setVertexAttribPointer("aPos", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0)
            .setVertexAttribPointer("aTexCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)))
            .enableVertexAttribArray("aPos")
            .enableVertexAttribArray("aTexCoord");
    
    this->defaultShader.setUniform("texture0", 0)
            .setUniform("texture1", 1);
    
    glActiveTexture(GL_TEXTURE0);
    this->texture0->bind();
    
    glActiveTexture(GL_TEXTURE1);
    this->texture1->bind();
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

bool Game::onMotionDown(const age::MotionEvent &event) {
    this->cam->onMotionDown(event);
    return true;
}

bool Game::onMotionMove(const age::MotionEvent &event) {
    this->cam->onMotionMove(event);
    return true;
}

bool Game::onMotionUp(const age::MotionEvent &event) {
    this->cam->onMotionUp(event);
    return true;
}

} // namespace age