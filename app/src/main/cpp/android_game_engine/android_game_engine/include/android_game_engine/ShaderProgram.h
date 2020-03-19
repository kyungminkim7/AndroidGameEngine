#pragma once
#include <string>

#include <GLES3/gl32.h>

#include <glm/fwd.hpp>

namespace age {

class UniformBuffer;

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
    
    ShaderProgram(ShaderProgram &&) = default;
    ShaderProgram& operator=(ShaderProgram &&) = default;
    
    ///
    /// \brief Sets this program as the current active shader program.
    ///
    /// This is a helper function that calls glUseProgram() on this shader program.
    ///
    void use();
    
    /// \name Uniforms
    /// Sets uniform value on this shader program. User must call ShaderProgram::use() before
    /// the 1st call to a ShaderProgram::setUniform() function to ensure that they are
    /// setting the uniform on the right active shader program.
    ///@{
    void setUniform(const std::string &name, bool value);
    void setUniform(const std::string &name, int value);
    void setUniform(const std::string &name, float value);
    void setUniform(const std::string &name, const glm::vec2 &v);
    void setUniform(const std::string &name, const glm::vec3 &v);
    void setUniform(const std::string &name, const glm::vec4 &v);
    void setUniform(const std::string &name, const glm::mat3 &m);
    void setUniform(const std::string &name, const glm::mat4 &m);
    ///@}

    ///
    /// \brief setUniformBlockBinding Links the uniform block of this shader to the binding point
    ///                               of the specified ubo.
    /// \param ubo Uniform Buffer Object to link against.
    ///
    void setUniformBlockBinding(const UniformBuffer &ubo);
    
private:
    unsigned int id;
};

} // namespace age
