#include <android_game_engine/Quadcopter.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <android_game_engine/VertexArray.h>
#include <android_game_engine/Utilities.h>

namespace age {

Quadcopter::Quadcopter(const std::string &modelFilepath, const Parameters &params)
    : GameObject(modelFilepath), mode(Mode::ACRO), maxRoll(params.maxRoll), maxPitch(params.maxPitch),
      maxRollRate(params.maxRollRate), maxPitchRate(params.maxPitchRate), maxYawRate(params.maxYawRate),
      maxThrust(params.maxThrust), maxAltitudeChangeRate(params.maxAltitudeChangeRate),
      rollController(params.angle_kp, params.angle_ki, params.angle_kd),
      pitchController(params.angle_kp, params.angle_ki, params.angle_kd),
      yawRateController(params.angleRate_kp, params.angleRate_ki, params.angleRate_kd),
      thrustController(params.thrust_kp, params.thrust_ki, params.thrust_kd),
      motors{Motor(0, params.motorRotationSpeed2Thrust),
           Motor(1, params.motorRotationSpeed2Thrust),
           Motor(2, params.motorRotationSpeed2Thrust),
           Motor(3, params.motorRotationSpeed2Thrust)} {
    auto k = params.controlRates2MotorRotationSpeed;
    this->controlRates2MotorRotationSpeeds = glm::mat4(k,  k,  k,  k,
                                                      -k,  k, -k,  k,
                                                      -k, -k,  k,  k,
                                                       k, -k, -k,  k);
    this->setMass(params.mass);
}

void Quadcopter::setMode(Mode mode) { this->mode = mode; }
void Quadcopter::setFloorAltitude(float altitude) { this->floorAltitude = altitude; }
void Quadcopter::setTargetAltitude(float altitude) { this->targetAltitude = altitude; }

void Quadcopter::onUpdate(std::chrono::duration<float> updateDuration) {
    if (this->mode == Mode::ANGLE) {
        // Calculate the roll and pitch rates to achieve target angles
        glm::vec3 z_ref(0.0f, 0.0f, 1.0f);
        auto y_ref = glm::cross(z_ref, this->getOrientationX());
        auto x_ref = glm::cross(y_ref, z_ref);
        glm::mat3 R_world_ref(x_ref, y_ref, z_ref);

        auto R_ref_body = glm::transpose(R_world_ref) * this->getOrientation();
        auto rpy = glm::eulerAngles(glm::toQuat(R_ref_body));

        this->controlRates[0] = this->rollController.computeCorrection(rpy[0], this->targetRoll, updateDuration);
        this->controlRates[1] = this->pitchController.computeCorrection(rpy[1], this->targetPitch, updateDuration);

        this->controlRates[0] = clip(this->controlRates[0], -this->maxRollRate, this->maxRollRate);
        this->controlRates[1] = clip(this->controlRates[1], -this->maxPitchRate, this->maxPitchRate);
    }

    // Calculate yaw correction to reach target yaw rate
    auto yawRate = this->getAngularVelocity().z;
    this->controlRates[2] = this->yawRateController.computeCorrection(yawRate, this->targetYawRate, updateDuration);
    this->controlRates[2] = clip(this->controlRates[2], -this->maxYawRate, this->maxYawRate);

    // Calculate thrust correction to reach target altitude
    this->targetAltitude += this->altitudeChangeRate * updateDuration.count();
    this->targetAltitude = std::max(this->floorAltitude + this->getScaledDimensions().z * 0.5f, this->targetAltitude);
    this->controlRates[3] = this->thrustController.computeCorrection(this->getPosition().z, this->targetAltitude, updateDuration)
            * this->getMass() * 9.80665f;
    this->controlRates[3] = clip(this->controlRates[3], 0.0f, this->maxThrust);

    // Apply correction to motors
    auto rotationSpeeds = this->controlRates2MotorRotationSpeeds * this->controlRates;

    // Cap motor speeds
    for (auto i = 0u; i < this->motors.size(); ++i) {
        switch (i) {
            case 0:
            case 3:
                if (rotationSpeeds[i] < 0.0f) rotationSpeeds[i] = 0.0f;
                break;

            case 1:
            case 2:
                rotationSpeeds[i] *= (rotationSpeeds[i] > 0.0f ? 0.0f : -1.0f);
                break;

            default:
                break;
            }

        motors[i].setRotationSpeed(rotationSpeeds[i]);
    }

    // Apply motor thrust
    const auto localZ = this->getOrientationZ();

    const auto frontLeftForce = localZ * this->motors[0].getThrust();
    const auto frontRightForce = localZ * this->motors[1].getThrust();
    const auto backRightForce = localZ * this->motors[2].getThrust();
    const auto backLeftForce = localZ * this->motors[3].getThrust();

    const auto halfDimensions = this->getScaledDimensions() / 2.0f;
    const auto orientation = this->getOrientation();

    this->applyForce(frontLeftForce, orientation * glm::vec3(halfDimensions.x, halfDimensions.y, halfDimensions.z));
    this->applyForce(frontRightForce, orientation * glm::vec3(halfDimensions.x, -halfDimensions.y, halfDimensions.z));
    this->applyForce(backRightForce, orientation * glm::vec3(-halfDimensions.x, -halfDimensions.y, halfDimensions.z));
    this->applyForce(backLeftForce, orientation * glm::vec3(-halfDimensions.x, halfDimensions.y, halfDimensions.z));

    // Apply motor moments
    const auto armLength = glm::length(glm::vec3{halfDimensions.x, halfDimensions.y, 0.0f});

    for (auto i = 0u; i < this->motors.size(); ++i) {
        const auto moment = armLength * this->motors[i].getThrust();
        this->applyTorque(glm::rotate(this->getLookAtDirection() * moment,
                                      glm::radians(-45.0f - 90.0f * i),
                                      localZ));
    }

    const auto yawMoment = armLength *
                           (-this->motors[0].getThrust() +
                            this->motors[1].getThrust() -
                            this->motors[2].getThrust() +
                            this->motors[3].getThrust());
    this->applyTorque(localZ * yawMoment);
}

void Quadcopter::onRollThrustInput(const glm::vec2 &input) {
    switch (this->mode) {
        case Mode::ACRO:
            this->controlRates[0] = input.x * this->maxRollRate;
            break;

        case Mode::ANGLE:
            this->targetRoll = input.x * this->maxRoll;
            break;

        default:
            break;
    }

    this->altitudeChangeRate = input.y * this->maxAltitudeChangeRate;
}

void Quadcopter::onYawPitchInput(const glm::vec2 &input) {
    switch (this->mode) {
        case Mode::ACRO:
            this->controlRates[1] = input.y * this->maxPitchRate;
            break;

        case Mode::ANGLE:
            this->targetPitch = input.y * this->maxPitch;
            break;

        default:
            break;
    }

    this->targetYawRate = -input.x * this->maxYawRate;
}

Quadcopter::Motor::Motor(int id, float rotationSpeed2Thrust) : id(id), rotationSpeed2Thrust(rotationSpeed2Thrust) {}
void Quadcopter::Motor::setRotationSpeed(float rotationSpeed) {this->rotationSpeed = rotationSpeed;}
float Quadcopter::Motor::getThrust() const {return this->rotationSpeed * this->rotationSpeed2Thrust;}


} // namespace age