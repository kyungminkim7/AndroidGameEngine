#include "TestGameAR.h"

#include <android_game_engine/LightDirectional.h>
#include <android_game_engine/Vehicle.h>

JNI_METHOD_DEFINITION(void, onSurfaceCreatedJNI) (JNIEnv *env, jobject activity,
                                                  int width, int height, int displayRotation) {
    age::GameEngine::onSurfaceCreated(width, height, displayRotation,
                                      std::make_unique<age::TestGameAR>());
}

JNI_METHOD_DEFINITION(void, onJoystickInputJNI)(JNIEnv *env, jobject gameActivity, float x, float y) {
    reinterpret_cast<age::TestGameAR*>(age::GameEngine::getGame())->onJoystickInput(x, y);
}

JNI_METHOD_DEFINITION(void, onResetJNI)(JNIEnv *env, jobject gameActivity) {
    reinterpret_cast<age::TestGameAR*>(age::GameEngine::getGame())->onReset();
}

namespace age {

void TestGameAR::onCreate() {
    GameAR::onCreate();

//    this->enablePhysicsDebugDrawer(true);
    this->getDirectionalLight()->setLookAtDirection({1.0f, 1.0f, -3.0f});

    this->atvCache = std::make_shared<Vehicle>("models/atv/ATV.3DS", 4.0f, 0.7f);
    this->atvCache->setScale(glm::vec3(0.2f));
    this->atvCache->setMass(1.0f);
}

void TestGameAR::onJoystickInput(float x, float y){
    if (this->atv != nullptr) {
        this->atv->onJoystickInput({x, y});
    }
}

void TestGameAR::onReset() {
    if (this->atv == nullptr) {
        return;
    }

    this->atv = nullptr;

    this->clearWorldList();
    this->setState(GameAR::State::TRACK_PLANES);

    GameEngine::callJavaActivityVoidMethod("arPlaneInitialized", "()V");
}

void TestGameAR::onGameObjectTouched(age::GameObject *gameObject, const glm::vec3 &touchPoint,
                                     const glm::vec3 &touchDirection, const glm::vec3 &touchNormal) {
    if (this->atv == nullptr) {
        this->atv = this->atvCache;
        this->atv->setPosition(touchPoint + glm::vec3(0.0f, 0.0f, 0.5f));
        this->atv->setOrientation(glm::mat3(1.0f));
        auto lookAtDirection = this->getCam()->getLookAtDirection();
        lookAtDirection.z = 0.025f;
        this->atv->setLookAtDirection(lookAtDirection);

        this->atv->clearForces();
        this->atv->setLinearVelocity(glm::vec3(0.0f));
        this->atv->setAngularVelocity(glm::vec3(0.0f));
        this->atv->applyCentralForce({0.0f, 0.0f, -1.0f});

        this->addToWorldList(this->atv);

        this->setState(GameAR::State::GAMEPLAY);

        GameEngine::callJavaActivityVoidMethod("gameInitialized", "()V");
    }
}

} // namespace age
