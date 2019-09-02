#pragma once

#include "Game.h"

#include <functional>
#include <memory>
#include <vector>

#include <arcore_c_api.h>

#include "ARCameraBackground.h"
#include "ShaderProgram.h"

namespace age {

class ARPlaneCircle;

class GameAR : public Game {
public:
    GameAR(JNIEnv *env, jobject javaApplicationContext, jobject javaActivityObject);
    ~GameAR();

    void onCreate() override;
    void onResume() override;
    void onPause() override;

    void onWindowChanged(int width, int height, int displayRotation) override;

    void onUpdate(std::chrono::duration<float> updateDuration) override;
    void render() override;

    bool onTouchDownEvent(float x, float y) override;
    bool onTouchMoveEvent(float x, float y) override;
    bool onTouchUpEvent(float x, float y) override;

protected:
    using StateRender = std::function<void()>;
    using StateOnTouch = std::function<bool(float, float)>;

    void setStateRender(StateRender stateRender);

    void setStateOnTouchDownEvent(StateOnTouch stateOnTouchDownEvent);
    void setStateOnTouchMoveEvent(StateOnTouch stateOnTouchMoveEvent);
    void setStateOnTouchUpEvent(StateOnTouch stateOnTouchUpEvent);

private:
    void updateCamera();
    void updatePlanes();
    void updateDirectionalLight();

    ShaderProgram arCameraBackgroundShader;
    ShaderProgram arPlaneShader;

    /// \name State
    /// AR Games will have at least 2 states:
    ///     1. Discovering the environment setting up the playing environment
    ///     2. Gameplay
    /// This will involve different render, and touch callback schemes.
    ///@{
    StateRender stateRender;

    StateOnTouch stateOnTouchDownEvent = [](float, float){return true;};
    StateOnTouch stateOnTouchMoveEvent = [](float, float){return true;};
    StateOnTouch stateOnTouchUpEvent = [](float, float){return true;};
    ///@}

    ArSession *arSession = nullptr;
    ArFrame *arFrame = nullptr;
    bool arInstallRequested = false;

    ARCameraBackground arCameraBackground;
    ArTrackingState arCameraTrackingState;

    std::vector<std::shared_ptr<ARPlaneCircle>> arPlanePool;
    int numActivePlanes = 0;
};

} // namespace age