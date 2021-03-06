#include <android_game_engine/PhysicsDebugDrawer.h>

#include <GLES3/gl32.h>
#include <glm/vec3.hpp>

#include <android_game_engine/Log.h>
#include <android_game_engine/ShaderProgram.h>

namespace age {

PhysicsDebugDrawer::PhysicsDebugDrawer(age::ShaderProgram *shader) : shader(shader) {
    float positions[] = {0.0f, 0.0f, 0.0f,
                         1.0f, 1.0f, 1.0f};
    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0u, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0u);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

PhysicsDebugDrawer::~PhysicsDebugDrawer() {
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void PhysicsDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to,
                                  const btVector3 &color) {
    this->shader->setUniform("origin", glm::vec3 (from.x(), from.y(), from.z()));
    this->shader->setUniform("basis", glm::vec3 (to.x() - from.x(),
                                                 to.y() - from.y(),
                                                 to.z() - from.z()));
    this->shader->setUniform("color", glm::vec3(color.x(), color.y(), color.z()));

    glBindVertexArray(this->vao);
    glDrawArrays(GL_LINES, 0, 2);
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB,
                                          btScalar distance, int lifeTime,
                                          const btVector3 &color) {}
                                          
void PhysicsDebugDrawer::reportErrorWarning(const char *warningString) {
    Log::error(warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3 &location, const char *textString) {}

void PhysicsDebugDrawer::setDebugMode(int debugMode) {this->debugMode = debugMode;}

} // namespace age
