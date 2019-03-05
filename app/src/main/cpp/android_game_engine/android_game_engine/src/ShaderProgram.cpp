#include <android_game_engine/ShaderProgram.h>

#include <array>
#include <memory>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/Exception.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerAssets.h>
#include <android_game_engine/Utilities.h>

namespace {
///
/// \brief compileShader Compiles a shader.
/// \param shaderType Type of shader to be created.
/// \param shaderPath Filepath of the shader's source code.
/// \return Compiled shader object.
/// \exception ge::BuildError Failed to compile shader.
///
unsigned int compileShader(unsigned int shaderType, const std::string &shaderPath);

///
/// \brief linkShaderProgram Links shaders to create a shader program.
///
/// This function does NOT delete the given shaders.
///
/// \param vertexShader Compiled vertex shader object.
/// \param fragmentShader Compiled fragment shader object.
/// \return Linked shader program.
/// \exception ge::BuildError Failed to link shader.
///                           This function does NOT delete the given shaders.
///
unsigned int linkShaderProgram(unsigned int vertexShader,
                               unsigned int fragmentShader);

unsigned int compileShader(unsigned int shaderType, const std::string &shaderPath) {
    auto shaderCode = age::ManagerAssets::readAsset(shaderPath);
    auto shaderCodeStr = reinterpret_cast<GLchar*>(shaderCode.data.get());
    std::array<GLint, 1> shaderCodeSize{shaderCode.size};
    
    // Compile shader
    auto shader = glCreateShader(shaderType);
    if (shader == 0) {
        throw age::BuildError("Failed to create shader for: " + shaderPath);
    }
    
    glShaderSource(shader, 1, &shaderCodeStr, shaderCodeSize.data());
    glCompileShader(shader);
    
    // Check for compilation errors
    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        // Get error log
        GLint logLength = 0;
        glGetShaderiv(shader,GL_INFO_LOG_LENGTH, &logLength);
        std::unique_ptr<char[]> compileLog(new char[logLength]);
        
        glGetShaderInfoLog(shader, logLength, nullptr, compileLog.get());
        
        std::stringstream errorMsg;
        errorMsg << "Failed to compile " << shaderPath << "\n" << compileLog;
        
        glDeleteShader(shader);
        
        throw age::BuildError(errorMsg.str());
    }
    
    return shader;
}

unsigned int linkShaderProgram(unsigned int vertexShader,
                               unsigned int fragmentShader) {
    auto shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    int linked;
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
    if (!linked) {
        // Get error log
        GLint logLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
        std::unique_ptr<char[]> linkLog(new char[logLength]);
        
        glGetProgramInfoLog(shaderProgram, logLength, nullptr, linkLog.get());
    
        glDetachShader(shaderProgram, vertexShader);
        glDetachShader(shaderProgram, fragmentShader);
        glDeleteProgram(shaderProgram);
        
        std::stringstream errorMsg;
        errorMsg << "Failed to link shaders\n" << linkLog;
        
        throw age::BuildError(errorMsg.str());
    }
    
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    
    return shaderProgram;
}

} // namespace

namespace age {
ShaderProgram::ShaderProgram(const std::string &vertexShaderPath,
                             const std::string &fragmentShaderPath) {
    // Compile shaders
    auto vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderPath);
    
    unsigned int fragmentShader;
    try {
        fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderPath);
    } catch (std::exception&) {
        glDeleteShader(vertexShader);
        throw;
    }
    
    // Link shaders
    try {
        this->id = linkShaderProgram(vertexShader, fragmentShader);
    } catch (std::exception&) {
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        throw;
    }
    Log::info("Successfully compiled and linked shaders:\n" +
              vertexShaderPath + "\n" + fragmentShaderPath);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(this->id);
}

ShaderProgram& ShaderProgram::use() {
    glUseProgram(this->id);
    return *this;
}

ShaderProgram& ShaderProgram::setVertexAttribPointer(const std::string &attribName, GLint size,
                                                     GLenum type, GLboolean normalized,
                                                     GLsizei stride, const GLvoid *ptr) {
    glVertexAttribPointer(static_cast<GLuint>(glGetAttribLocation(this->id, attribName.c_str())),
            size, type, normalized, stride, ptr);
    return *this;
}

ShaderProgram& ShaderProgram::enableVertexAttribArray(const std::string &attribName) {
    glEnableVertexAttribArray(static_cast<GLuint>(glGetAttribLocation(this->id, attribName.c_str())));
    return *this;
}

ShaderProgram& ShaderProgram::disableVertexAttribArray(const std::string &attribName) {
    glDisableVertexAttribArray(static_cast<GLuint>(glGetAttribLocation(this->id, attribName.c_str())));
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, const glm::vec2 &v) {
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), v.x, v.y);
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, const glm::vec3 &v) {
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), v.x, v.y, v.z);
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, const glm::mat3 &m) {
    glUniformMatrix3fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
    return *this;
}

ShaderProgram& ShaderProgram::setUniform(const std::string &name, const glm::mat4 &m) {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
    return *this;
}
} // namespace age