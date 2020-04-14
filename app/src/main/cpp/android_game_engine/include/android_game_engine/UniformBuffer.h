#pragma once

#include <string>

namespace age {

class UniformBuffer {
public:
    ///
    /// \brief UniformBuffer Creates and allocates a uniform buffer object of
    ///                      specified size and links it against a generated
    ///                      binding point.
    /// \param size_bytes Number of bytes to allocate in the uniform buffer
    ///                   object.
    ///
    UniformBuffer(const std::string &uniformBlockName, unsigned int size_bytes);

    ///
    /// Releases its binding point to allow it to be linked with another
    /// UniformBuffer object and deletes the uniform buffer object on the GPU.
    ///
    ~UniformBuffer();

    UniformBuffer(UniformBuffer &&) noexcept = default;
    UniformBuffer& operator=(UniformBuffer &&) noexcept = default;

    std::string getUniformBlockName() const;

    ///
    /// \brief getBindingPoint Returns the binding point this object is linked
    ///                        against for use in linking uniform blocks against.
    /// \return The binding point.
    ///
    unsigned int getBindingPoint() const;

    ///
    /// \brief bufferSubData Sets the data within this uniform buffer object.
    /// \param offset_bytes Offset in bytes into the uniform buffer object's data
    ///                     store where the data replacement will begin.
    /// \param size_bytes Size in bytes of data being replaced.
    /// \param data Pointer to data to copy into this uniform buffer object.
    ///
    void bufferSubData(unsigned int offset_bytes, unsigned int size_bytes, const void *data);

private:
    std::string uniformBlockName;
    unsigned int ubo;
    unsigned int bindingPoint;
};

} // namespace age