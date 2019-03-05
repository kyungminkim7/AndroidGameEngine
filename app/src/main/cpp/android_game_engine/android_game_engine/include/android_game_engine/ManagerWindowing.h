//
// Created by Kyung on 2/28/2019.
//

#include <vector>

#include <android/native_window.h>

#include <EGL/egl.h>

namespace age {
namespace ManagerWindowing {

void init(ANativeWindow *window);
void shutdown();

void swapBuffers();

void setEglAttribs(const std::vector<EGLint> &attribs);
void setContextAttribs(const std::vector<EGLint> &attribs);

int getWindowWidth();
int getWindowHeight();

} // namespace ManagerWindowing
} // namespace age