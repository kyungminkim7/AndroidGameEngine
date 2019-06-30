#include <android_game_engine/ManagerWindowing.h>

#include <android_game_engine/Log.h>
#include <android_game_engine/Exception.h>

namespace {

EGLDisplay display = EGL_NO_DISPLAY;
EGLContext context = EGL_NO_CONTEXT;
EGLSurface surface = EGL_NO_SURFACE;

std::vector<EGLint> eglAttribs {
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_DEPTH_SIZE, 24,
    EGL_NONE
};

std::vector<EGLint> contextAttribs {
    EGL_CONTEXT_CLIENT_VERSION, 3,
    EGL_NONE
};

int windowWidth = 0;
int windowHeight = 0;

} // namespace

namespace age {
namespace ManagerWindowing {

void init(ANativeWindow *window) {
    // Initialize display.
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        throw WindowingSystemError("Failed to obtain EGL display connection.");
    } else {
        Log::info("Opened EGL display connection.");
    }
    
    EGLint eglMajorVersion, eglMinorVersion;
    if (!eglInitialize(display, &eglMajorVersion, &eglMinorVersion)) {
        throw WindowingSystemError("Failed to initialize EGL.");
    } else {
        Log::info("Initialized EGL " +
                  std::to_string(eglMajorVersion) + "." + std::to_string(eglMinorVersion));
    }
    
    // Obtain surface config.
    EGLint numConfigs;
    EGLConfig config;
    if (!eglChooseConfig(display, eglAttribs.data(), &config, 1, &numConfigs) ||
        numConfigs <= 0) {
        throw WindowingSystemError("Failed to obtain appropriate EGL config.");
    } else {
        Log::info("Found appropriate EGL config.");
    }
    
    // Create surface.
    surface = eglCreateWindowSurface(display, config, window, nullptr);
    if (surface == EGL_NO_SURFACE) {
        throw WindowingSystemError("Failed to create window surface.");
    } else {
        Log::info("Created EGL window surface.");
    }
    
    // Obtain GL context.
    context = eglCreateContext(display, config, nullptr, contextAttribs.data());
    if (context == EGL_NO_CONTEXT) {
        throw WindowingSystemError("Failed to create context: " + std::to_string(eglGetError()));
    } else {
        Log::info("Created GL context.");
    }
    
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        throw WindowingSystemError("Failed to make context current.");
    } else {
        Log::info("Made GL context current.");
    }
    
    // Obtain window dimensions.
    eglQuerySurface(display, surface, EGL_WIDTH, &windowWidth);
    eglQuerySurface(display, surface, EGL_HEIGHT, &windowHeight);
    if (windowWidth <= 0 || windowHeight <= 0) {
        throw WindowingSystemError("Failed to obtain valid window dimensions.");
    }
}

void shutdown() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
            context = EGL_NO_CONTEXT;
        }
        
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
            surface = EGL_NO_SURFACE;
        }
        
        eglTerminate(display);
        display = EGL_NO_DISPLAY;
    }
    
    windowWidth = 0;
    windowHeight = 0;
}

void swapBuffers() { eglSwapBuffers(display, surface); }

void setEglAttribs(const std::vector<EGLint> &attribs) { eglAttribs = attribs; }
void setContextAttribs(const std::vector<EGLint> &attribs) { contextAttribs = attribs; }

int getWindowWidth() { return windowWidth; }
int getWindowHeight() { return windowHeight; }

}
} // namespace age