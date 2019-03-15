#pragma once

#include <vector>

#include <glm/fwd.hpp>

namespace age {

///
/// \brief Wrapper class for OpenGL Element Buffer Object.
///
class ElementBufferObject {
public:
    explicit ElementBufferObject(const std::vector<glm::uvec3> &indices);
    ~ElementBufferObject();
    
    void bind();
    
    size_t getNumIndices() const;
    
private:
    unsigned int ebo;
    size_t numIndices;
};

inline size_t ElementBufferObject::getNumIndices() const {return this->numIndices;}

} // namespace age