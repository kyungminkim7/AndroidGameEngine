#include <android_game_engine/JNIInterface.h>

#include <vector>

#include <android_game_engine/Exception.h>

namespace {

constexpr auto JNI_VERSION = JNI_VERSION_1_6;
JavaVM *javaVM = nullptr;
jobject jActivityGlobal;
jobject jContextGlobal;

// JNI methods
void onCreateJNI(JNIEnv *env, jobject activity, jobject context, jobject assetManager);
void onStartJNI(JNIEnv *env, jobject activity);
void onResumeJNI(JNIEnv *env, jobject activity);
void onPauseJNI(JNIEnv *env, jobject activity);
void onStopJNI(JNIEnv *env, jobject activity);
void onDestroyJNI(JNIEnv *env, jobject activity);
void onSurfaceChangedJNI(JNIEnv *env, jobject activity, int width, int height, int displayRotation);
void updateJNI(JNIEnv *env, jobject activity);
void onTouchDownEventJNI(JNIEnv *env, jobject activity, float x, float y);
void onTouchMoveEventJNI(JNIEnv *env, jobject activity, float x, float y);
void onTouchUpEventJNI(JNIEnv *env, jobject activity, float x, float y);

void onCreateJNI(JNIEnv *env, jobject activity,
                 jobject context, jobject assetManager) {
    age::JNIInterface::init(env, activity, context);
    age::GameEngine::onCreate(env, assetManager);
}

void onStartJNI(JNIEnv *env, jobject activity) {
    age::GameEngine::onStart();
}

void onResumeJNI(JNIEnv *env, jobject activity) {
    age::GameEngine::onResume();
}

void onPauseJNI(JNIEnv *env, jobject activity) {
    age::GameEngine::onPause();
}

void onStopJNI(JNIEnv *env, jobject activity) {
    age::GameEngine::onStop();
}

void onDestroyJNI(JNIEnv *env, jobject activity) {
    age::GameEngine::onDestroy();
}

void onSurfaceChangedJNI(JNIEnv *env, jobject activity,
                         int width, int height, int displayRotation) {
    age::GameEngine::onSurfaceChanged(width, height, displayRotation);
}

void updateJNI(JNIEnv *env, jobject activity) {
    age::GameEngine::update();
}

void onTouchDownEventJNI(JNIEnv *env, jobject activity, float x, float y) {
    age::GameEngine::onTouchDownEvent(x, y);
}

void onTouchMoveEventJNI(JNIEnv *env, jobject activity, float x, float y) {
    age::GameEngine::onTouchMoveEvent(x, y);
}

void onTouchUpEventJNI(JNIEnv *env, jobject activity, float x, float y) {
    age::GameEngine::onTouchUpEvent(x, y);
}

} // namespace

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    auto env = age::JNIInterface::getJNIEnv();

    auto c = env->FindClass(JNI_ENV_CLASS_PATH);
    if (c == nullptr) return JNI_ERR;

    const std::vector<JNINativeMethod> methods = {
        {"onCreateJNI", "(Landroid/content/Context;Landroid/content/res/AssetManager;)V",
            reinterpret_cast<void *>(onCreateJNI)},
        {"onStartJNI", "()V", reinterpret_cast<void *>(onStartJNI)},
        {"onResumeJNI", "()V", reinterpret_cast<void *>(onResumeJNI)},
        {"onPauseJNI", "()V", reinterpret_cast<void *>(onPauseJNI)},
        {"onStopJNI", "()V", reinterpret_cast<void *>(onStopJNI)},
        {"onDestroyJNI", "()V", reinterpret_cast<void *>(onDestroyJNI)},
        {"onSurfaceChangedJNI", "(III)V", reinterpret_cast<void *>(onSurfaceChangedJNI)},
        {"updateJNI", "()V", reinterpret_cast<void *>(updateJNI)},
        {"onTouchDownEventJNI", "(FF)V", reinterpret_cast<void *>(onTouchDownEventJNI)},
        {"onTouchMoveEventJNI", "(FF)V", reinterpret_cast<void *>(onTouchMoveEventJNI)},
        {"onTouchUpEventJNI", "(FF)V", reinterpret_cast<void *>(onTouchUpEventJNI)}
    };
    auto rc = env->RegisterNatives(c, methods.data(), methods.size());
    if (rc != JNI_OK) return rc;

    return JNI_VERSION;
}

namespace age {
namespace JNIInterface {

void init(JNIEnv *env, jobject activity, jobject context) {
    jActivityGlobal = env->NewGlobalRef(activity);
    jContextGlobal = env->NewGlobalRef(context);
}

void shutdown() {
    auto env = getJNIEnv();
    env->DeleteGlobalRef(jActivityGlobal);
    env->DeleteGlobalRef(jContextGlobal);
    javaVM = nullptr;
}

JNIEnv *getJNIEnv() {
    JNIEnv *env;
    if (javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION) != JNI_OK) {
        throw JNIError("Failed to obtain JNIEnv from javaVM");
    }
    return env;
}

jobject getActivity() { return jActivityGlobal; }
jobject getContext() { return jContextGlobal; }

} // namespace JNIInterface
} // namespace age

