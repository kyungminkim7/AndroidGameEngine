#include <android_game_engine/GameAR.h>

#include <android_game_engine/Exception.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/ARPlaneCircle.h>

namespace age {

GameAR::GameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject) :
    Game(env, javaApplicationContext, javaActivityObject),
    arCameraBackgroundShader("shaders/ARCameraBackground.vert", "shaders/ARCameraBackground.frag"),
    arPlaneShader("shaders/ARPlane.vert", "shaders/ARPlane.frag") {
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

    this->updatePlanes();
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

    this->getCam()->setPosition(camPose[3]);
    this->getCam()->setOrientation(camPose);

//    auto position = this->getCam()->getPosition();
//    auto look = this->getCam()->getOrientationX();
//    Log::info("Cam position: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z));
//    Log::info("Cam look: " + std::to_string(look.x) + ", " + std::to_string(look.y) + ", " + std::to_string(look.z));

    // Convert from OpenGL camera frame
    this->getCam()->rotate(glm::radians(90.0f), this->getCam()->getOrientationZ());
    this->getCam()->rotate(glm::radians(90.0f), this->getCam()->getOrientationY());
}

void GameAR::updatePlanes() {
    ArTrackableList *planeList = nullptr;
    ArTrackableList_create(this->arSession, &planeList);

    ArSession_getAllTrackables(this->arSession, AR_TRACKABLE_PLANE, planeList);

    int32_t numPlanes = 0;
    ArTrackableList_getSize(this->arSession, planeList, &numPlanes);

    while (this->arPlanePool.size() < numPlanes) {
        this->arPlanePool.emplace_back(std::make_shared<ARPlaneCircle>(Texture2D("images/trigrid.png")));
    }
    auto prevNumActivePlanes = this->numActivePlanes;
    this->numActivePlanes = 0;

    for (auto i = 0; i < numPlanes; ++i) {
        ArTrackable *plane = nullptr;
        ArTrackableList_acquireItem(this->arSession, planeList, i, &plane);

        ArPlane *subsumePlane = nullptr;
        ArPlane_acquireSubsumedBy(this->arSession, ArAsPlane(plane), &subsumePlane);
        if (!subsumePlane) {
            ArTrackingState trackingState;
            ArTrackable_getTrackingState(this->arSession, plane, &trackingState);
            if (trackingState == ArTrackingState::AR_TRACKING_STATE_TRACKING) {
                // Found a unique plane, extract the pose and dimensions
                ArPose *pose;
                ArPose_create(this->arSession, nullptr, &pose);
                ArPlane_getCenterPose(this->arSession, ArAsPlane(plane), pose);

                glm::mat4 planePose;
                ArPose_getMatrix(this->arSession, pose, glm::value_ptr(planePose));

                ArPose_destroy(pose);

                float width, length;
                ArPlane_getExtentX(this->arSession, ArAsPlane(plane), &width);
                ArPlane_getExtentZ(this->arSession, ArAsPlane(plane), &length);

                // Adjust the next available plane in the plane pool to reflect the extracted data
                auto planeCircle = this->arPlanePool[this->numActivePlanes++].get();
                if (this->numActivePlanes > prevNumActivePlanes) {
                    this->registerPhysics(planeCircle);
                }

                planeCircle->setPosition(planePose[3]);
                planeCircle->setOrientation(planePose);

                // Convert from OpenGL frame
                planeCircle->rotate(glm::radians(90.0f), planeCircle->getOrientationZ());
                planeCircle->rotate(glm::radians(90.0f), planeCircle->getOrientationY());

                planeCircle->setDiameter(std::min(width, length));
            }
        }

        ArTrackable_release(ArAsTrackable(subsumePlane));
        ArTrackable_release(plane);
    }

    for (auto i = this->numActivePlanes; i < prevNumActivePlanes; ++i) {
        this->unregisterPhysics(this->arPlanePool[i].get());
    }

    ArTrackableList_destroy(planeList);
}

void GameAR::render() {
    if (this->arSession == nullptr) return;

    // Render camera image in background
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    this->arCameraBackgroundShader.use();

    int64_t arFrameTimestamp;
    ArFrame_getTimestamp(this->arSession, this->arFrame, &arFrameTimestamp);

    this->arCameraBackground.render(&this->arCameraBackgroundShader, arFrameTimestamp);

    // Don't render world scene if camera is not tracking
    if (this->arCameraTrackingState != AR_TRACKING_STATE_TRACKING) return;

    // Render world scene
    glViewport(0, 0, ManagerWindowing::getWindowWidth(), ManagerWindowing::getWindowHeight());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
    this->renderWorld();

    // Render planes
    glDepthMask(GL_FALSE);
    this->arPlaneShader.use();
    this->arPlaneShader.setUniform("projection_view",
                                   this->getCam()->getProjectionMatrix() * this->getCam()->getViewMatrix());
    for (auto i = 0u; i < this->numActivePlanes; ++i) {
        this->arPlanePool[i]->render(&this->arPlaneShader);
    }
    glDepthMask(GL_TRUE);
}

void GameAR::setStateRender(age::GameAR::StateRender stateRender) {
    this->stateRender = stateRender;
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