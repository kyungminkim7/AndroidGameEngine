#include <android_game_engine/ShaderProgram.h>

#include <memory>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <android_game_engine/Exception.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/Shader.h>
#include <android_game_engine/UniformBuffer.h>

namespace age {
ShaderProgram::ShaderProgram(const std::string &vertexShaderPath,
                             const std::string &fragmentShaderPath) :
                             program(new unsigned int(glCreateProgram()),
                                     [](unsigned int *program){ glDeleteProgram(*program); delete program; }){
    // Compile shaders
    auto shaderDeleter = [program=*this->program](Shader *shader) {
        shader->detachFromProgram(program);
        delete shader;
    };

    std::unique_ptr<Shader, decltype(shaderDeleter)> vertexShader(
            new Shader(vertexShaderPath, GL_VERTEX_SHADER), shaderDeleter);
    vertexShader->attachToProgram(*this->program);

    std::unique_ptr<Shader, decltype(shaderDeleter)> fragmentShader(
            new Shader(fragmentShaderPath, GL_FRAGMENT_SHADER), shaderDeleter);
    fragmentShader->attachToProgram(*this->program);

    // Link shaders
    int linked;
    glLinkProgram(*this->program);
    glGetProgramiv(*this->program, GL_LINK_STATUS, &linked);
    if (!linked) {
        // Get error log
        GLint logLength = 0;
        glGetProgramiv(*this->program, GL_INFO_LOG_LENGTH, &logLength);
        std::unique_ptr<char[]> linkLog(new char[logLength]);
        glGetProgramInfoLog(*this->program, logLength, nullptr, linkLog.get());

        std::stringstream errorMsg;
        errorMsg << "Failed to link shaders\n" << linkLog;

        throw age::BuildError(errorMsg.str());
    }

    Log::info("Successfully compiled and linked shaders:\n" +
              vertexShaderPath + "\n" + fragmentShaderPath);
}

void ShaderProgram::use() {
    glUseProgram(*this->program);
}

void ShaderProgram::setUniform(const std::string &name, bool value) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const std::string &name, int value) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniform1i(location, value);
}

void ShaderProgram::setUniform(const std::string &name, float value) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniform1f(location, value);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec2 &v) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniform2f(location, v.x, v.y);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec3 &v) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniform3f(location, v.x, v.y, v.z);
}

void ShaderProgram::setUniform(const std::string &name, const glm::vec4 &v) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniform4f(location, v.x, v.y, v.z, v.w);
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat3 &m) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::setUniform(const std::string &name, const glm::mat4 &m) {
    auto location = glGetUniformLocation(*this->program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(m));
}

void ShaderProgram::setUniformBlockBinding(const UniformBuffer &ubo) {
    glUniformBlockBinding(*this->program,
                          glGetUniformBlockIndex(*this->program, ubo.getUniformBlockName().c_str()),
                          ubo.getBindingPoint());
}

} // namespace age