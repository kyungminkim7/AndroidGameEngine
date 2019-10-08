#include <android_game_engine/GameAR.h>

#include <glm/gtx/string_cast.hpp>

#include <android_game_engine/Exception.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/ARPlane.h>

namespace {
const auto T_game_android = glm::rotate(glm::mat4(1.0f),
                                        glm::radians(-90.0f),
                                        {0.0f, 1.0f, 0.0f}) *
                            glm::rotate(glm::mat4(1.0f),
                                        glm::radians(-90.0f),
                                        {0.0f, 0.0f, 1.0f});

const auto R_ar_game = static_cast<glm::mat3>(glm::rotate(glm::mat4(1.0f),
                                                          glm::radians(90.0f),
                                                          {0.0f, 0.0f, 1.0f}) *
                                              glm::rotate(glm::mat4(1.0f),
                                                          glm::radians(90.0f),
                                                          {0.0f, 1.0f, 0.0f}));
} // namespace

namespace age {

GameAR::GameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject) :
    Game(env, javaApplicationContext, javaActivityObject),
    arCameraBackgroundShader("shaders/ARCameraBackground.vert", "shaders/ARCameraBackground.frag"),
    arPlaneShader("shaders/ARPlane.vert", "shaders/ARPlane.frag"),
    arPlaneShadowedShader("shaders/ARPlaneShadowed.vert", "shaders/ARPlaneShadowed.frag") {

    this->bindToProjectionViewUBO(&this->arPlaneShader);

    this->bindToProjectionViewUBO(&this->arPlaneShadowedShader);
    this->bindToLightSpaceUBO(&this->arPlaneShadowedShader);

    // Enable blending for transparent plane indicators
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

GameAR::~GameAR() {
    if (this->arSession != nullptr) {
        ArSession_destroy(this->arSession);
    }

    if (this->arFrame != nullptr) {
        ArFrame_destroy(this->arFrame);
    }
}

void GameAR::onCreate() {
    Game::onCreate();
}

void GameAR::onResume() {
    Game::onResume();

    if (this->arSession == nullptr) {
        auto env = this->getJNIEnv();
        auto activity = this->getJavaActivityObject();
        ArInstallStatus installStatus;

        // If install was not yet requested, that means that we are resuming the
        // activity first time because of explicit user interaction (such as
        // launching the application)
        if (ArCoreApk_requestInstall(env, activity, !this->arInstallRequested, &installStatus) != AR_SUCCESS) {
            throw ARError("Failed to request ARCore install.");
        }

        if (installStatus == AR_INSTALL_STATUS_INSTALL_REQUESTED) {
            this->arInstallRequested = true;
            return;
        }

        if (ArSession_create(env, this->getJavaApplicationContext(), &this->arSession) != AR_SUCCESS) {
            throw ARError("Failed to create AR session.");
        }

        ArFrame_create(this->arSession, &this->arFrame);
        if (!this->arFrame) {
            throw ARError("Failed to create arFrame");
        }

        ArSession_setDisplayGeometry(this->arSession, ManagerWindowing::getDisplayRotation(),
                                     ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    }

    if (ArSession_resume(this->arSession) != AR_SUCCESS) {
        throw ARError("Failed to resume AR session.");
    }

    // Enable environmental HDR lighting
    ArConfig *config = nullptr;
    ArConfig_create(this->arSession, &config);
    ArConfig_setLightEstimationMode(this->arSession, config, AR_LIGHT_ESTIMATION_MODE_ENVIRONMENTAL_HDR);
    ArSession_configure(this->arSession, config);
    ArConfig_destroy(config);
}

void GameAR::onPause() {
    Game::onPause();
    if (this->arSession != nullptr) {
        ArSession_pause(this->arSession);
    }
}

void GameAR::onWindowChanged(int width, int height, int displayRotation) {
    Game::onWindowChanged(width, height, displayRotation);

    if (this->arSession != nullptr) {
        ArSession_setDisplayGeometry(this->arSession, ManagerWindowing::getDisplayRotation(),
                                     ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    }
}

void GameAR::onUpdate(std::chrono::duration<float> updateDuration) {
    Game::onUpdate(updateDuration);

    if (this->arSession == nullptr) return;

    // Update AR session to get current frame
    ArSession_setCameraTextureName(this->arSession, this->arCameraBackground.getTexture());
    ArSession_update(this->arSession, this->arFrame);

    this->updateCamera();

    this->arCameraBackground.onUpdate(this->arSession, this->arFrame);

    if (this->arCameraTrackingState != AR_TRACKING_STATE_TRACKING) return;

    this->updateDirectionalLight();

    if (this->state == State::TRACK_PLANES) {
        this->updatePlanes();
    }
}

void GameAR::updateCamera() {
    ArCamera *cam;
    ArFrame_acquireCamera(this->arSession, this->arFrame, &cam);

    ArCamera_getProjectionMatrix(this->arSession, cam,
                                 this->getCam()->getNearPlane(),this->getCam()->getFarPlane(),
                                 const_cast<float*>(this->getCam()->getProjectionMatrixPtr()));

    ArPose *pose;
    ArPose_create(this->arSession, nullptr, &pose);
    ArCamera_getDisplayOrientedPose(this->arSession, cam, pose);

    glm::mat4 camPose;
    ArPose_getMatrix(this->arSession, pose, glm::value_ptr(camPose));

    ArPose_destroy(pose);

    ArCamera_getTrackingState(this->arSession, cam, &this->arCameraTrackingState);

    ArCamera_release(cam);

    camPose = T_game_android * camPose;
    this->getCam()->setPosition(camPose[3]);
    this->getCam()->setOrientation(static_cast<glm::mat3>(camPose) * R_ar_game);
}

void GameAR::updatePlanes() {
    ArTrackableList *planeList = nullptr;
    ArTrackableList_create(this->arSession, &planeList);

    ArSession_getAllTrackables(this->arSession, AR_TRACKABLE_PLANE, planeList);

    int32_t numPlanes = 0;
    ArTrackableList_getSize(this->arSession, planeList, &numPlanes);

    glm::mat4 floorPose;
    float floorWidth, floorLength;

    for (auto i = 0; i < numPlanes; ++i) {
        ArTrackable *arPlane = nullptr;
        ArTrackableList_acquireItem(this->arSession, planeList, i, &arPlane);

        ArPlane *subsumePlane = nullptr;
        ArPlane_acquireSubsumedBy(this->arSession, ArAsPlane(arPlane), &subsumePlane);
        if (!subsumePlane) {
            ArTrackingState trackingState;
            ArTrackable_getTrackingState(this->arSession, arPlane, &trackingState);
            if (trackingState == ArTrackingState::AR_TRACKING_STATE_TRACKING) {
                // Found a unique plane, extract the pose and dimensions
                ArPose *pose;
                ArPose_create(this->arSession, nullptr, &pose);
                ArPlane_getCenterPose(this->arSession, ArAsPlane(arPlane), pose);

                glm::mat4 planePose;
                ArPose_getMatrix(this->arSession, pose, glm::value_ptr(planePose));

                ArPose_destroy(pose);

                float planeWidth, planeLength;
                ArPlane_getExtentX(this->arSession, ArAsPlane(arPlane), &planeWidth);
                ArPlane_getExtentZ(this->arSession, ArAsPlane(arPlane), &planeLength);

                planePose = T_game_android * planePose;

                // Choose the lowest plane as the floor
                if (i == 0 || planePose[3].z < floorPose[3].z) {
                    floorPose = planePose;
                    floorWidth = planeWidth;
                    floorLength = planeLength;
                }
            }
        }

        ArTrackable_release(ArAsTrackable(subsumePlane));
        ArTrackable_release(arPlane);
    }

    ArTrackableList_destroy(planeList);

    // Update floor
    if (numPlanes > 0) {
        if (this->floor == nullptr) {
            this->floor = std::make_shared<ARPlane>(Texture2D("images/trigrid.png"));
            this->registerPhysics(this->floor.get());

            // Signal to Java activity that a plane was found
            auto env = this->getJNIEnv();
            auto activityClass = env->GetObjectClass(this->getJavaActivityObject());
            auto callback = env->GetMethodID(activityClass, "arPlaneFound", "()V");
            env->CallVoidMethod(this->getJavaActivityObject(), callback);
        }

        this->floor->setDimensions({floorLength, floorWidth});

        this->floor->setOrientation(static_cast<glm::mat3>(floorPose) * R_ar_game);
        this->floor->setPosition(floorPose[3]);
        this->floor->translateInLocalFrame(glm::vec3(0.0f, 0.0f, -this->floor->getScaledDimensions().z * 0.5f));

        this->floor->setCollisionDiameter(50.0f);
    }
}

void GameAR::updateDirectionalLight() {
    ArLightEstimate* lightEstimate;
    ArLightEstimate_create(this->arSession, &lightEstimate);
    ArFrame_getLightEstimate(this->arSession, this->arFrame, lightEstimate);

    ArLightEstimateState lightEstimateState;
    ArLightEstimate_getState(this->arSession, lightEstimate, &lightEstimateState);

    if (lightEstimateState == AR_LIGHT_ESTIMATE_STATE_VALID) {
//        glm::vec3 lightDirection;
//        ArLightEstimate_getEnvironmentalHdrMainLightDirection(this->arSession, lightEstimate,
//                                                              glm::value_ptr(lightDirection));
//        lightDirection = static_cast<glm::mat3>(T_game_android) * -lightDirection;
//        if (lightDirection.z < 0.0f) {
//            this->getDirectionalLight()->setLookAtDirection(lightDirection);
//        }

        glm::vec3 lightColor;
        ArLightEstimate_getEnvironmentalHdrMainLightIntensity(this->arSession, lightEstimate,
                                                              glm::value_ptr(lightColor));
        this->getDirectionalLight()->setAmbient(lightColor * 0.1f);
        this->getDirectionalLight()->setDiffuse(lightColor * 0.4f);
        this->getDirectionalLight()->setSpecular(lightColor * 0.8f);
    }

    ArLightEstimate_destroy(lightEstimate);
}

void GameAR::render() {
    if (this->arSession == nullptr) return;

    this->updateUBOs();

    // Render camera image in background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    this->arCameraBackgroundShader.use();

    int64_t arFrameTimestamp;
    ArFrame_getTimestamp(this->arSession, this->arFrame, &arFrameTimestamp);

    this->arCameraBackground.render(&this->arCameraBackgroundShader, arFrameTimestamp);

    // Don't render world scene if camera is not tracking
    if (this->arCameraTrackingState != AR_TRACKING_STATE_TRACKING) return;

    // Render world scene
    this->renderShadowMapSetup();
    this->renderShadowMap();

    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
    this->renderWorld();

    // Render planes
    if (this->floor != nullptr) {
        auto floorShader = (this->state == State::TRACK_PLANES) ?
                &this->arPlaneShader : &this->arPlaneShadowedShader;

        glDepthMask(GL_FALSE);
        floorShader->use();
        this->bindShadowMap(floorShader);
        this->floor->render(floorShader);

        glDepthMask(GL_TRUE);
    }
}

bool GameAR::onTouchDownEvent(float x, float y) {
    return Game::onTouchDownEvent(x, y) && this->stateOnTouchDownEvent(x, y);
}

bool GameAR::onTouchMoveEvent(float x, float y) {
    return Game::onTouchMoveEvent(x, y) && this->stateOnTouchMoveEvent(x, y);
}

bool GameAR::onTouchUpEvent(float x, float y) {
    return Game::onTouchUpEvent(x, y) && this->stateOnTouchUpEvent(x, y);
}

void GameAR::setState(age::GameAR::State state) {
    this->state = state;

    if (this->floor != nullptr) {
        switch (state) {
            case State::TRACK_PLANES:
                this->floor->setDimensions(this->floorDimensions);
                this->floor->setCollisionDiameter(50.0f);
                break;

            case State::GAMEPLAY:
                this->floorDimensions = this->floor->getScaledDimensions();
                this->floor->setDimensions(glm::vec2(50.0f));
                break;
        }
    }
}

void GameAR::setStateOnTouchDownEvent(age::GameAR::StateOnTouch stateOnTouchDownEvent) {
    this->stateOnTouchDownEvent = stateOnTouchDownEvent;
}

void GameAR::setStateOnTouchMoveEvent(age::GameAR::StateOnTouch stateOnTouchMoveEvent) {
    this->stateOnTouchMoveEvent = stateOnTouchMoveEvent;
}

void GameAR::setStateOnTouchUpEvent(age::GameAR::StateOnTouch stateOnTouchUpEvent) {
    this->stateOnTouchUpEvent = stateOnTouchUpEvent;
}

} // namespace age