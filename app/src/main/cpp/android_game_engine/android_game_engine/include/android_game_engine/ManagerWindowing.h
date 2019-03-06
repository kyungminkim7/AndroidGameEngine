#pragma once

/**
 * Singleton Windowing Manager. This init should be the first thing called before
 * initializing any other managers, instantiating games, or calling any EGL or
 * Open GL ES functions. This shutdown should also be the last thing called after
 * shutting down all other games or managers.
 */

#include <vector>

#include <android/native_window.h>

#include <EGL/egl.h>

namespace age {
namespace ManagerWindowing {

void init(ANativeWindow *window);
void shutdown();

void swapBuffers();

/**
 * Use this function before init or calls to GameEngine::run to specify EGL
 * attributes different from the default.
 * @param attribs
 */
void setEglAttribs(const std::vector<EGLint> &attribs);

/**
 * Use this function before init or calls to GameEngine::run to specify
 * OpenGL context attributes different from the default.
 * @param attribs
 */
void setContextAttribs(const std::vector<EGLint> &attribs);

int getWindowWidth();
int getWindowHeight();

} // namespace ManagerWindowing
} // namespace age