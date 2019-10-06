#include "TestGame.h"

#include <array>
#include <functional>
#include <memory>

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/trigonometric.hpp>

#include <android_game_engine/ARPlane.h>
#include <android_game_engine/Box.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Quadcopter.h>

namespace age {

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI)
    (JNIEnv *env, jobject gameActivity, jobject gameApplicationContext,
     int windowWidth, int windowHeight, int displayRotation, jobject j_asset_manager) {
    GameEngineJNI::init(env, windowWidth, windowHeight, displayRotation, j_asset_manager);
    GameEngineJNI::onCreate(std::make_unique<TestGame>(env, gameApplicationContext, gameActivity));
}

JNI_METHOD_DEFINITION(void, onRollThrustInputJNI)(JNIEnv *env, jobject gameActivity, float roll, float thrust) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onRollThrustInput(roll, thrust);
}

JNI_METHOD_DEFINITION(void, onYawPitchInputJNI)(JNIEnv *env, jobject gameActivity, float yaw, float pitch) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onYawPitchInput(yaw, pitch);
}

JNI_METHOD_DEFINITION(void, onResetUAVJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<TestGame*>(GameEngineJNI::getGame())->onResetUAV();
}

TestGame::TestGame(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject)
    : BaseGameType(env, javaApplicationContext, javaActivityObject) {}

void TestGame::onCreate() {
    BaseGameType::onCreate();
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

    {
        // Create UAV
        Quadcopter::Parameters params;
        params.mass = 1.0f;

        params.maxRoll = glm::radians(35.0f);
        params.maxPitch = glm::radians(35.0f);

        params.maxRollRate = glm::radians(360.0f);
        params.maxPitchRate = glm::radians(360.0f);
        params.maxYawRate = glm::radians(120.0f);
        params.maxThrust = 10.0f;

        params.controlRates2MotorRotationSpeed = 150.0f;

        params.angle_kp = 0.8f;
        params.angle_ki = 0.0f;
        params.angle_kd = 0.2f;

        params.angleRate_kp = 1.0f;
        params.angleRate_ki = 0.0f;
        params.angleRate_kd = 0.0f;

        params.motorRotationSpeed2Thrust = 2.0E-3f;

        this->uavCache = std::make_shared<Quadcopter>("models/parrot/drone.3ds", params);
        this->uavCache->setLabel("UAV");

        this->uavCache->setMode(Quadcopter::Mode::ANGLE);
        this->uavCache->setDamping(0.25f, 0.05f);

        this->random = std::make_shared<GameObject>("models/X47B_UCAV_3DS/X47B_UCAV_v08.3ds");
        this->random->setScale(glm::vec3(2.0f));
        this->random->setMass(1.0f);
    }
}

void TestGame::onRollThrustInput(float roll, float thrust) {
    if (this->uav != nullptr) {
        this->uav->onRollThrustInput({roll, thrust});
    }
}

void TestGame::onYawPitchInput(float yaw, float pitch) {
    if (this->uav != nullptr) {
        this->uav->onYawPitchInput({yaw, pitch});
    }
}

void TestGame::onResetUAV() {
    this->uav = nullptr;

    this->clearWorldList();
    this->setState(GameAR::State::TRACK_PLANES);

    auto env = this->getJNIEnv();
    auto activityClass = env->GetObjectClass(this->getJavaActivityObject());
    auto callback = env->GetMethodID(activityClass, "arPlaneFound", "()V");
    env->CallVoidMethod(this->getJavaActivityObject(), callback);
}

void TestGame::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
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
        this->uav->setPosition(touchPoint + glm::vec3(0.0f, 0.0f, 0.05f));
        this->uav->setOrientation(glm::mat3(1.0f));
        auto lookAtDirection = this->getCam()->getLookAtDirection();
        lookAtDirection.z = 0.025f;
        this->uav->setLookAtDirection(lookAtDirection);
        this->uav->clearForces();
        this->uav->applyCentralForce({0.0f, 0.0f, -1.0f});
        this->addToWorldList(this->uav);

        auto positionOffset = glm::normalize(glm::vec3(lookAtDirection.x, lookAtDirection.z, 0.0f));
        positionOffset = glm::rotateZ(positionOffset * 0.5f, glm::radians(20.0f));
        positionOffset.z = 1.0f;
        this->random->setPosition(touchPoint + positionOffset);
        this->random->rotate(glm::radians(45.0f), {1.0f, 0.0f, 1.0f});
        this->random->applyCentralForce({0.0f, 0.0f, -1.0f});
        this->addToWorldList(this->random);

        this->setState(GameAR::State::GAMEPLAY);

        auto env = this->getJNIEnv();
        auto activityClass = env->GetObjectClass(this->getJavaActivityObject());
        auto callback = env->GetMethodID(activityClass, "uavCreated", "()V");
        env->CallVoidMethod(this->getJavaActivityObject(), callback);
    }
}

} // namespace age
