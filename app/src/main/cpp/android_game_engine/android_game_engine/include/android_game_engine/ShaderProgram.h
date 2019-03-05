#pragma once
#include <string>

#include <GLES2/gl2.h>

#include <glm/fwd.hpp>

namespace age {

///
/// \brief Manages loading, compiling, linking and working with shader programs.
///
class ShaderProgram
{
public:
    ///
    /// \brief Loads, compiles, and links given shaders into an OpenGL shader program.
    /// \param[in] vertexShaderPath Filepath of the vertex shader.
    /// \param[in] fragmentShaderPath Filepath of the fragment shader.
    /// \exception age::BuildError Failed to compile or link shaders.
    ///
    ShaderProgram(const std::string &vertexShaderPath,
                  const std::string &fragmentShaderPath);
    
    ///
    /// \brief Deletes the OpenGL shader program from the GPU.
    ///
    ~ShaderProgram();
    
    ShaderProgram(const ShaderProgram &) = delete;
    ShaderProgram(ShaderProgram &&) = delete;
    ShaderProgram& operator=(const ShaderProgram &) = delete;
    ShaderProgram& operator=(ShaderProgram &&) = delete;
    
    ///
    /// \brief Sets this program as the current active shader program.
    ///
    /// This is a helper function that calls glUseProgram() on this shader program.
    ///
    ShaderProgram& use();
    
    ShaderProgram& setVertexAttribPointer(const std::string &attribName, GLint size, GLenum type,
                                          GLboolean normalized, GLsizei stride, const GLvoid *ptr);
    ShaderProgram& enableVertexAttribArray(const std::string &attribName);
    ShaderProgram& disableVertexAttribArray(const std::string &attribName);
    
    /// \name Uniforms
    /// Sets uniform value on this shader program. User must call ShaderProgram::use() before
    /// the 1st call to a ShaderProgram::setUniform() function to ensure that they are
    /// setting the uniform on the right active shader program.
    ///@{
    ShaderProgram& setUniform(const std::string &name, bool value);
    ShaderProgram& setUniform(const std::string &name, int value);
    ShaderProgram& setUniform(const std::string &name, float value);
    ShaderProgram& setUniform(const std::string &name, float x, float y, float z);
    ShaderProgram& setUniform(const std::string &name, float x, float y, float z, float w);
    ShaderProgram& setUniform(const std::string &name, const glm::vec2 &v);
    ShaderProgram& setUniform(const std::string &name, const glm::vec3 &v);
    ShaderProgram& setUniform(const std::string &name, const glm::mat3 &m);
    ShaderProgram& setUniform(const std::string &name, const glm::mat4 &m);
    ///@}
    
private:
    unsigned int id;
};

} // namespace age