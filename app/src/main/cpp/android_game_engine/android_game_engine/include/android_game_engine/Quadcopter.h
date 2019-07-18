#pragma once

#include "GameObject.h"

#include <array>

#include <glm/mat4x4.hpp>

#include "PID.h"

namespace age {

class Quadcopter : public GameObject {
public:
    enum class Mode {ACRO, ANGLE};

    struct Parameters {
        float mass;

        float maxRoll;
        float maxPitch;
        float maxRollRate;
        float maxPitchRate;
        float maxYawRate;
        float maxThrust;

        float controlRates2MotorRotationSpeed;

        // Angle PID
        float angle_kp;
        float angle_ki;
        float angle_kd;

        // Motor attributes
        float motorRotationSpeed2Thrust;
    };

    explicit Quadcopter(const std::string &modelFilepath, const Parameters &params);

    void setMode(Mode mode);
    
    void onUpdate(std::chrono::duration<float> updateDuration) override;
    
    void onRollThrustInput(const glm::vec2 &input);
    void onYawPitchInput(const glm::vec2 &input);

private:
    class Motor {
    public:
        Motor(float rotationSpeed2Thrust);

        void setRotationSpeed(float rotationSpeed);
        float getThrust() const;

    private:
        float rotationSpeed = 0.0f;
        float rotationSpeed2Thrust;
    };

    Mode mode;

    float maxRoll;
    float maxPitch;
    float maxRollRate;
    float maxPitchRate;
    float maxYawRate;
    float maxThrust;

    float targetRoll = 0.0f;
    float targetPitch = 0.0f;

    glm::vec4 controlRates = glm::vec4(0.0f); ///< rpy rates, thrust
    glm::mat4 controlRates2MotorRotationSpeeds;

    PID rollController;
    PID pitchController;

    std::array<Motor, 4> motors; ///< Motor order: front left, front right, back right, back left
};

} // namespace age