#include <android_game_engine/GameEngine.h>

#include <chrono>
#include <utility>

#include <android_game_engine/Exception.h>
#include <android_game_engine/Game.h>
#include <android_game_engine/ManagerAssets.h>
#include <android_game_engine/ManagerWindowing.h>

namespace {

std::unique_ptr<age::Game> game = nullptr;

// JNI variables/methods
constexpr auto JNI_VERSION = JNI_VERSION_1_6;
JavaVM *javaVM = nullptr;
jclass jActivityClassRef;
jobject jActivityRef;
jobject jContextRef;
jobject jAssetManagerRef;

JNIEnv *getJNIEnv();
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

void onCreateJNI(JNIEnv *env, jobject activity, jobject context, jobject assetManager) {
    jActivityRef = env->NewGlobalRef(activity);
    jContextRef = env->NewGlobalRef(context);
    jAssetManagerRef = env->NewGlobalRef(assetManager);
    age::ManagerAssets::init(env, jAssetManagerRef);
}

void onStartJNI(JNIEnv *env, jobject activity) { if (game) game->onStart(); }
void onResumeJNI(JNIEnv *env, jobject activity) { if (game) game->onResume(); }
void onPauseJNI(JNIEnv *env, jobject activity) { game->onPause(); }

void onStopJNI(JNIEnv *env, jobject activity) {
    game->onStop();
    age::ManagerWindowing::shutdown();
}

void onDestroyJNI(JNIEnv *env, jobject activity) {
    game->onDestroy();
    game = nullptr;

    age::ManagerAssets::shutdown();
    env->DeleteGlobalRef(jAssetManagerRef);
    env->DeleteGlobalRef(jContextRef);
    env->DeleteGlobalRef(jActivityRef);
}

void onSurfaceChangedJNI(JNIEnv *env, jobject activity,
                         int width, int height, int displayRotation) {
    age::ManagerWindowing::init(width, height, displayRotation);
    game->onWindowChanged(width, height, displayRotation);
}

void updateJNI(JNIEnv *env, jobject activity) {
    static std::chrono::system_clock::time_point lastUpdateTime = std::chrono::system_clock::now();

    auto currentUpdateTime = std::chrono::system_clock::now();
    auto updateDuration = currentUpdateTime - lastUpdateTime;
    lastUpdateTime = currentUpdateTime;

    game->onUpdate(updateDuration);
    game->render();
}

void onTouchDownEventJNI(JNIEnv *env, jobject activity, float x, float y) {
    game->onTouchDownEvent(x, y);
}

void onTouchMoveEventJNI(JNIEnv *env, jobject activity, float x, float y) {
    game->onTouchMoveEvent(x, y);
}

void onTouchUpEventJNI(JNIEnv *env, jobject activity, float x, float y) {
    game->onTouchUpEvent(x, y);
}

JNIEnv *getJNIEnv() {
    JNIEnv *env;
    if (javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION) != JNI_OK) {
        throw age::JNIError("Failed to obtain JNIEnv from javaVM");
    }
    return env;
}

} // namespace

// Register native methods
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    auto env = getJNIEnv();

    auto activityClass = env->FindClass(JNI_ENV_CLASS_PATH);
    if (activityClass == nullptr) return JNI_ERR;
    jActivityClassRef = reinterpret_cast<jclass>(env->NewGlobalRef(activityClass));

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
    auto result = env->RegisterNatives(jActivityClassRef, methods.data(), methods.size());
    return result == JNI_OK ? JNI_VERSION : result;
}

namespace age {
namespace GameEngine {

Game *getGame() {
    return game.get();
}

void onSurfaceCreated(int width, int height, int displayRotation, std::unique_ptr<Game> &&g) {
    ManagerWindowing::init(width, height, displayRotation);
    game = std::move(g);

    game->onCreate();
    game->onStart();
    game->onResume();
}

void callJavaActivityVoidMethod(const std::string &methodName, const std::string &signature) {
    auto env = getJNIEnv();
    auto method = env->GetMethodID(jActivityClassRef, methodName.c_str(), signature.c_str());
    env->CallVoidMethod(jActivityRef, method);
}

} // namespace GameEngine
} // namespace age