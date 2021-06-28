#include <android_game_engine/Shader.h>

#include <array>
#include <sstream>

#include <android_game_engine/Asset.h>
#include <android_game_engine/Exception.h>
#include <android_game_engine/ManagerAssets.h>

namespace age {

Shader::Shader(const std::string &filepath, GLenum type) :
    shader(new GLuint(glCreateShader(type)),
           [](GLuint *shader){ glDeleteShader(*shader); delete shader; }) {
    auto asset = age::ManagerAssets::openAsset(filepath);
    auto length = asset.getLength();
    std::unique_ptr<GLchar[]> buffer(new GLchar[length]);
    if (asset.read(buffer.get(), length) < 0) {
        throw age::LoadError("Failed to read shader code from file: " + filepath);
    }

    // Compile shader
    auto shaderCode = buffer.get();
    std::array<GLint, 1> shaderCodeSize{static_cast<GLint>(length)};
    glShaderSource(*this->shader, 1, &shaderCode, shaderCodeSize.data());
    glCompileShader(*this->shader);

    // Check for compilation errors
    int compiled;
    glGetShaderiv(*this->shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        // Get error log
        GLint logLength = 0;
        glGetShaderiv(*this->shader,GL_INFO_LOG_LENGTH, &logLength);
        std::unique_ptr<char[]> compileLog(new char[logLength]);

        glGetShaderInfoLog(*this->shader, logLength, nullptr, compileLog.get());

        std::stringstream errorMsg;
        errorMsg << "Failed to compile " << filepath << "\n" << compileLog;

        throw age::BuildError(errorMsg.str());
    }
}

void Shader::attachToProgram(unsigned int program) {
    glAttachShader(program, *this->shader);
}

void Shader::detachFromProgram(unsigned int program) {
    glDetachShader(program, *this->shader);
}

} // namespace age