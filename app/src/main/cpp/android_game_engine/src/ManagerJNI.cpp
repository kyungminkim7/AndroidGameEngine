#include <android_game_engine/ManagerJNI.h>

#include <android_game_engine/Exception.h>

namespace {

JavaVM *javaVM = nullptr;
jobject jActivityGlobal;
jobject jContextGlobal;

} // namespace

namespace age {
namespace ManagerJNI {

void init(JNIEnv *env, jobject activity, jobject context) {
    if (env->GetJavaVM(&javaVM) != JNI_OK) {
        throw JNIError("Failed to obtain Java VM.");
    }

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
    if (javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        throw JNIError("Failed to obtain JNIEnv from javaVM");
    }
    return env;
}

jobject getActivity() { return jActivityGlobal; }
jobject getContext() { return jContextGlobal; }

} // namespace ManagerJNI
} // namespace age