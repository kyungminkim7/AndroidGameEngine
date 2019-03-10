#include <android_game_engine/ElementBufferObject.h>

#include <GLES2/gl2.h>

namespace age {

ElementBufferObject::ElementBufferObject(const std::vector<unsigned int> &indices) {
    this->numIndices = indices.size();
    
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numIndices * sizeof(unsigned int),
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