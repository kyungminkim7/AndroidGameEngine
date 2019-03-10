#pragma once

#include <vector>

namespace age {

///
/// \brief Wrapper class for OpenGL Element Buffer Object.
///
class ElementBufferObject {
public:
    ElementBufferObject(const std::vector<unsigned int> &indices);
    ~ElementBufferObject();
    
    void bind();
    
    size_t getNumIndices() const;
    
private:
    unsigned int ebo;
    size_t numIndices;
};

inline size_t ElementBufferObject::getNumIndices() const {return this->numIndices;}

} // namespace age