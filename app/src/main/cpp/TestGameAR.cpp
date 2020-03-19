#include "TestGameAR.h"

#include <android_game_engine/Box.h>

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
    (JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
     int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, displayRotation, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<TestGameAR>(env, gameApplicationContext, gameActivity));
}

JNI_METHOD_DEFINITION(void, onLeftJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<TestGameAR*>(GameEngineJNI::getGame())->onLeftJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onRightJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<TestGameAR*>(GameEngineJNI::getGame())->onRightJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onResetJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<TestGameAR*>(GameEngineJNI::getGame())->onReset();
}

TestGameAR::TestGameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject)
    : GameAR(env, javaApplicationContext, javaActivityObject) {}

void TestGameAR::onCreate() {
    GameAR::onCreate();

//    this->enablePhysicsDebugDrawer(true);
    this->getDirectionalLight()->setLookAtDirection({1.0f, 1.0f, -3.0f});

    this->boxCache = std::shared_ptr<Box>(new Box({Texture2D(glm::vec3(1.0f, 0.0f, 0.0f))},
                                                  {Texture2D(glm::vec3(1.0f))}));
//    this->boxCache = std::make_shared<GameObject>("models/parrot/drone.3ds");
    this->boxCache->setScale(glm::vec3(0.1f));
    this->boxCache->setMass(1.0f);
}

void TestGameAR::onLeftJoystickInput(float x, float y){ }

void TestGameAR::onRightJoystickInput(float x, float y){ }

void TestGameAR::onReset() {
    if (this->box == nullptr) {
        return;
    }

    this->box = nullptr;

    this->clearWorldList();
    this->setState(GameAR::State::TRACK_PLANES);

    // Signal back to Java GameActivityAR
    auto env = this->getJNIEnv();
    auto activityClass = env->GetObjectClass(this->getJavaActivityObject());
    auto callback = env->GetMethodID(activityClass, "arPlaneInitialized", "()V");
    env->CallVoidMethod(this->getJavaActivityObject(), callback);
}

void TestGameAR::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                     const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    // Set UAV position
    if (this->box == nullptr) {
        this->box = this->boxCache;
        this->box->setPosition(touchPoint + glm::vec3(0.0f, 0.0f, 0.5f));
        this->box->setOrientation(glm::mat3(1.0f));
        auto lookAtDirection = this->getCam()->getLookAtDirection();
        lookAtDirection.z = 0.025f;
        this->box->setLookAtDirection(lookAtDirection);

        this->box->clearForces();
        this->box->setLinearVelocity(glm::vec3(0.0f));
        this->box->setAngularVelocity(glm::vec3(0.0f));
        this->box->applyCentralForce({0.0f, 0.0f, -1.0f});

        this->addToWorldList(this->box);

        this->setState(GameAR::State::GAMEPLAY);

        // Signal back to Java GameActivityAR
        auto env = this->getJNIEnv();
        auto activityClass = env->GetObjectClass(this->getJavaActivityObject());
        auto callback = env->GetMethodID(activityClass, "gameInitialized", "()V");
        env->CallVoidMethod(this->getJavaActivityObject(), callback);
    }
}

} // namespace age
