#include <android_game_engine/ShadowMap.h>

#include <GLES3/gl32.h>

#include <android_game_engine/Exception.h>

namespace age {

ShadowMap::ShadowMap(unsigned int width, unsigned int height) : width(width), height(height) {
    // Generate color buffer
    glGenTextures(1, &this->colorBuffer);
    glBindTexture(GL_TEXTURE_2D, this->colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Generate depth and stencil buffer for shadow map
    glGenTextures(1, &this->depthStencilBuffer);
    glBindTexture(GL_TEXTURE_2D, this->depthStencilBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width, this->height, 0,
                 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Attach buffers to fbo
    glGenFramebuffers(1, &this->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorBuffer, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->depthStencilBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw Error("Failed to build complete FBO for shadow map.");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

ShadowMap::~ShadowMap() {
    glDeleteFramebuffers(1, &this->fbo);
    glDeleteTextures(1, &this->depthStencilBuffer);
    glDeleteTextures(1, &this->colorBuffer);
}

void ShadowMap::bindFramebuffer() {glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);}
void ShadowMap::bindDepthMap() {glBindTexture(GL_TEXTURE_2D, this->depthStencilBuffer);}

}