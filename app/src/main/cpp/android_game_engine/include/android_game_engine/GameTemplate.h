#pragma once

#include <chrono>

#include <jni.h>

namespace age {

/**
 * Users should subclass GameTemplate and then run it using GameEngine::run with the derived
 * GameTemplate class.
 */
class GameTemplate {
public:
    GameTemplate(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);
    virtual ~GameTemplate();

    GameTemplate(GameTemplate &&) = default;
    GameTemplate& operator=(GameTemplate &&) = default;

    virtual void onCreate();
    virtual void onStart();
    virtual void onResume();
    virtual void onPause();
    virtual void onStop();
    virtual void onDestroy();

    virtual void onWindowChanged(int width, int height, int displayRotation);

    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    virtual void render() = 0;

    virtual bool onTouchDownEvent(float x, float y);
    virtual bool onTouchMoveEvent(float x, float y);
    virtual bool onTouchUpEvent(float x, float y);

protected:
    JNIEnv* getJNIEnv();
    jobject getJavaApplicationContext();
    jobject getJavaActivityObject();

private:
    JavaVM *javaVM;
    jobject javaApplicationContext;
    jobject javaActivityObject;
};

} // namespace age
