#pragma once

#include <functional>
#include <memory>
#include <string>

#include <GLES3/gl32.h>

namespace age {

class Shader {
public:
    Shader(const std::string &filepath, GLenum type);

    void attachToProgram(unsigned int program);
    void detachFromProgram(unsigned int program);

private:
    std::unique_ptr<GLuint, std::function<void(GLuint *)>> shader;
};

} // namespace age
