#pragma once

namespace age {

///
/// \brief Implements depth mapping for generating shadows.
///
class ShadowMap {
public:
    ShadowMap(unsigned int width, unsigned int height);

    ~ShadowMap();

    ShadowMap(ShadowMap &&) = default;
    ShadowMap& operator=(ShadowMap &&) = default;

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    ///
    /// \brief Sets the current framebuffer to the one associated with the shadow map.
    ///
    /// This should be used for the first pass in drawing game objects to calculate the depth map.
    ///
    void bindFramebuffer();

    ///
    /// \brief Binds the generated depth map as a GL_TEXTURE_2D texture.
    ///
    /// This should be used on the 2nd pass after generating the depth map to use for calculating
    /// and drawing shadows.
    ///
    void bindDepthMap();

private:
    unsigned int width;
    unsigned int height;

    unsigned int fbo;
    unsigned int colorBuffer;
    unsigned int depthStencilBuffer;
};

inline unsigned int ShadowMap::getWidth() const {return this->width;}
inline unsigned int ShadowMap::getHeight() const {return this->height;}

} // namespace age