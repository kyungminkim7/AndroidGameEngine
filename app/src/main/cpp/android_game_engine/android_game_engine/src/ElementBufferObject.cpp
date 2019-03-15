#include <android_game_engine/ElementBufferObject.h>

#include <GLES2/gl2.h>
#include <glm/vec3.hpp>

namespace age {

ElementBufferObject::ElementBufferObject(const std::vector<glm::uvec3> &indices) {
    this->numIndices = indices.size() * 3;
    
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec3),
                 indices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

ElementBufferObject::~ElementBufferObject() {
    glDeleteBuffers(1, &this->ebo);
}

void ElementBufferObject::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
}

} // namespace age