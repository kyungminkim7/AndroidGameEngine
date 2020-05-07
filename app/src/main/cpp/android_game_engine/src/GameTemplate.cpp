#include <android_game_engine/GameTemplate.h>

#include <android_game_engine/Exception.h>

namespace age {

GameTemplate::GameTemplate(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject) {
    if (env->GetJavaVM(&this->javaVM) != JNI_OK) throw JNIError("Failed to obtain Java VM.");
    this->javaApplicationContext = env->NewGlobalRef(javaApplicationContext);
    this->javaActivityObject = env->NewGlobalRef(javaActivityObject);
}

GameTemplate::~GameTemplate() {
    auto env = this->getJNIEnv();
    env->DeleteGlobalRef(this->javaActivityObject);
    env->DeleteGlobalRef(this->javaApplicationContext);
}

JNIEnv* GameTemplate::getJNIEnv() {
    JNIEnv *env;
    if (javaVM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        throw JNIError("Failed to obtain JNIEnv from javaVM");
    }
    return env;
}

jobject GameTemplate::getJavaApplicationContext() {return this->javaApplicationContext;}
jobject GameTemplate::getJavaActivityObject() {return this->javaActivityObject;}

void GameTemplate::onCreate() {}
void GameTemplate::onStart() {}
void GameTemplate::onResume() {}
void GameTemplate::onPause() {}
void GameTemplate::onStop() {}
void GameTemplate::onDestroy() {}
void GameTemplate::onWindowChanged(int width, int height, int displayRotation) {}
void GameTemplate::onUpdate(std::chrono::duration<float> updateDuration) {}
bool GameTemplate::onTouchDownEvent(float x, float y) {return true;}
bool GameTemplate::onTouchMoveEvent(float x, float y) {return true;}
bool GameTemplate::onTouchUpEvent(float x, float y) {return true;}

} // namespace age