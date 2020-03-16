#include "TestGameAR.h"

#include <array>
#include <functional>
#include <memory>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/trigonometric.hpp>

#include <android_game_engine/Box.h>
#include <android_game_engine/ManagerWindowing.h>

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
    (JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
     int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, displayRotation, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<TestGameAR>(env, gameApplicationContext, gameActivity));
}

JNI_METHOD_DEFINITION(void, onRollThrustInputJNI)(JNIEnv *env, jobject gameActivity, float roll, float thrust) {
    reinterpret_cast<TestGameAR*>(GameEngineJNI::getGame())->onRollThrustInput(roll, thrust);
}

JNI_METHOD_DEFINITION(void, onYawPitchInputJNI)(JNIEnv *env, jobject gameActivity, float yaw, float pitch) {
    reinterpret_cast<TestGameAR*>(GameEngineJNI::getGame())->onYawPitchInput(yaw, pitch);
}

JNI_METHOD_DEFINITION(void, onResetUAVJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<TestGameAR*>(GameEngineJNI::getGame())->onResetUAV();
}

TestGameAR::TestGameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject)
    : GameAR(env, javaApplicationContext, javaActivityObject) {}

void TestGameAR::onCreate() {
    GameAR::onCreate();
//    this->enablePhysicsDebugDrawer(true);
    this->getDirectionalLight()->setLookAtDirection({1.0f, 1.0f, -3.0f});

//    this->getCam()->setPosition({-0.5f, 0.0f, 1.0f});
//    this->getCam()->setLookAtPoint({0.5f, 0.0f, 0.7f});

//    {
//        const auto scale = 50.0f;
//        std::shared_ptr<Box> floor(new Box({Texture2D("images/wood.png")},
//                                           {Texture2D(glm::vec3(1.0f))},
//                                           glm::vec2(scale)));
//
//        floor->setLabel("Floor");
//        floor->setScale(glm::vec3{scale, scale, 0.2f});
//        floor->setPosition(glm::vec3(0.0f));
//        floor->setSpecularExponent(32.0f);
//        floor->setFriction(1.0f);
//        this->addToWorldList(floor);
//    }

    this->uavCache = std::shared_ptr<Box>(new Box({Texture2D(glm::vec3(1.0f, 0.0f, 0.0f))},
                                                  {Texture2D(glm::vec3(1.0f))}));
//    this->uavCache = std::make_shared<GameObject>("models/parrot/drone.3ds");
    this->uavCache->setScale(glm::vec3(0.1f));
    this->uavCache->setMass(1.0f);
}

void TestGameAR::onRollThrustInput(float roll, float thrust) { }

void TestGameAR::onYawPitchInput(float yaw, float pitch) { }

void TestGameAR::onResetUAV() {
    if (this->uav == nullptr) {
        return;
    }

    this->uav = nullptr;

    this->clearWorldList();
    this->setState(GameAR::State::TRACK_PLANES);

    auto env = this->getJNIEnv();
    auto activityClass = env->GetObjectClass(this->getJavaActivityObject());
    auto callback = env->GetMethodID(activityClass, "arPlaneFound", "()V");
    env->CallVoidMethod(this->getJavaActivityObject(), callback);
}

void TestGameAR::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                     const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
//    if (this->uav == nullptr) {
//        this->uav = this->uavCache;
//        this->addToWorldList(this->uav);
//    }
//    this->uav->setPosition(touchPoint + glm::vec3(0.0f, 0.0f, 0.2f));
//    this->uav->setOrientation(glm::mat3(1.0f));
//    this->uav->clearForces();
//    this->uav->applyCentralForce({0.0f, 0.0f, -1.0f});

    // Set UAV position
    if (this->uav == nullptr) {
        this->uav = this->uavCache;
        this->uav->setPosition(touchPoint + glm::vec3(0.0f, 0.0f, 0.5f));
        this->uav->setOrientation(glm::mat3(1.0f));
        auto lookAtDirection = this->getCam()->getLookAtDirection();
        lookAtDirection.z = 0.025f;
        this->uav->setLookAtDirection(lookAtDirection);
        this->uav->clearForces();
        this->uav->applyCentralForce({0.0f, 0.0f, -1.0f});
        this->addToWorldList(this->uav);

        this->setState(GameAR::State::GAMEPLAY);

        auto env = this->getJNIEnv();
        auto activityClass = env->GetObjectClass(this->getJavaActivityObject());
        auto callback = env->GetMethodID(activityClass, "uavCreated", "()V");
        env->CallVoidMethod(this->getJavaActivityObject(), callback);
    }
}

} // namespace age
