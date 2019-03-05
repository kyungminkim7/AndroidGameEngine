#include <android_game_engine/Game.h>

#include <GLES2/gl2.h>

#include <android_game_engine/ManagerWindowing.h>

namespace {
float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
        0.0f,  0.5f, 0.0f  // top
};
} // namespace

namespace age {

Game::Game() : defaultShader("shaders/default.vert", "shaders/default.frag"){}

void Game::init() {
    glViewport(0, 0, ManagerWindowing::getWindowWidth(),ManagerWindowing::getWindowHeight());
    
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    this->defaultShader.setVertexAttribPointer("aPos", 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)
         .enableVertexAttribArray("aPos");
    
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Game::onUpdate(std::chrono::duration<float> updateDuration) {

}

void Game::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    this->defaultShader.use();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    this->defaultShader.setVertexAttribPointer("aPos", 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0)
            .enableVertexAttribArray("aPos");
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

} // namespace age