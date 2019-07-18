#include <android_game_engine/Quadcopter.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>

#include <android_game_engine/Log.h>
#include <android_game_engine/VertexArray.h>
#include <android_game_engine/Utilities.h>

namespace age {

const std::vector<glm::vec3> positions {
        // back face
        {-0.5f, -0.5f, -0.5f}, // bottom-left
        {0.5f,  0.5f, -0.5f}, // top-right
        {0.5f, -0.5f, -0.5f}, // bottom-right
        {-0.5f,  0.5f, -0.5f}, // top-left
        // front face
        {-0.5f, -0.5f,  0.5f}, // bottom-left
        {0.5f, -0.5f,  0.5f}, // bottom-right
        {0.5f,  0.5f,  0.5f}, // top-right
        {-0.5f,  0.5f,  0.5f}, // top-left
        // left face
        {-0.5f,  0.5f,  0.5f}, // top-right
        {-0.5f,  0.5f, -0.5f}, // top-left
        {-0.5f, -0.5f, -0.5f}, // bottom-left
        {-0.5f, -0.5f,  0.5f}, // bottom-right
        // right face
        {0.5f,  0.5f,  0.5f}, // top-left
        {0.5f, -0.5f, -0.5f}, // bottom-right
        {0.5f,  0.5f, -0.5f}, // top-right
        {0.5f, -0.5f,  0.5f}, // bottom-left
        // bottom face
        {-0.5f, -0.5f, -0.5f}, // top-right
        {0.5f, -0.5f, -0.5f}, // top-left
        {0.5f, -0.5f,  0.5f}, // bottom-left
        {-0.5f, -0.5f,  0.5f}, // bottom-right
        // top face
        {-0.5f,  0.5f, -0.5f},  // top-left
        {0.5f,  0.5f , 0.5f},  // bottom-right
        {0.5f,  0.5f, -0.5f},  // top-right
        {-0.5f,  0.5f,  0.5f}  // bottom-left
};

const std::vector<glm::vec3> normals {
        // back face
        {0.0f,  0.0f, -1.0f}, // bottom-left
        {0.0f,  0.0f, -1.0f}, // top-right
        {0.0f,  0.0f, -1.0f}, // bottom-right
        {0.0f,  0.0f, -1.0f}, // top-left
        // front face
        {0.0f,  0.0f,  1.0f}, // bottom-left
        {0.0f,  0.0f,  1.0f}, // bottom-right
        {0.0f,  0.0f,  1.0f}, // top-right
        {0.0f,  0.0f,  1.0f}, // top-left
        // left face
        {-1.0f,  0.0f,  0.0f}, // top-right
        {-1.0f,  0.0f,  0.0f}, // top-left
        {-1.0f,  0.0f,  0.0f}, // bottom-left
        {-1.0f,  0.0f,  0.0f}, // bottom-right
        // right face
        {1.0f,  0.0f,  0.0f}, // top-left
        {1.0f,  0.0f,  0.0f}, // bottom-right
        {1.0f,  0.0f,  0.0f}, // top-right
        {1.0f,  0.0f,  0.0f}, // bottom-left
        // bottom face
        {0.0f, -1.0f,  0.0f}, // top-right
        {0.0f, -1.0f,  0.0f}, // top-left
        {0.0f, -1.0f,  0.0f}, // bottom-left
        {0.0f, -1.0f,  0.0f}, // bottom-right
        // top face
        {0.0f,  1.0f,  0.0f}, // top-left
        {0.0f,  1.0f,  0.0f}, // bottom-right
        {0.0f,  1.0f,  0.0f}, // top-right
        {0.0f,  1.0f,  0.0f}  // bottom-left
};

const std::vector<glm::vec2> textureCoords {
        // back face
        {0.0f, 0.0f}, // bottom-left
        {1.0f, 1.0f}, // top-right
        {1.0f, 0.0f}, // bottom-right
        {0.0f, 1.0f}, // top-left
        // front face
        {0.0f, 0.0f}, // bottom-left
        {1.0f, 0.0f}, // bottom-right
        {1.0f, 1.0f}, // top-right
        {0.0f, 1.0f}, // top-left
        // left face
        {1.0f, 0.0f}, // top-right
        {1.0f, 1.0f}, // top-left
        {0.0f, 1.0f}, // bottom-left
        {0.0f, 0.0f}, // bottom-right
        // right face
        {1.0f, 0.0f}, // top-left
        {0.0f, 1.0f}, // bottom-right
        {1.0f, 1.0f}, // top-right
        {0.0f, 0.0f}, // bottom-left
        // bottom face
        {0.0f, 1.0f}, // top-right
        {1.0f, 1.0f}, // top-left
        {1.0f, 0.0f}, // bottom-left
        {0.0f, 0.0f}, // bottom-right
        // top face
        {0.0f, 1.0f}, // top-left
        {1.0f, 0.0f}, // bottom-right
        {1.0f, 1.0f}, // top-right
        {0.0f, 0.0f}  // bottom-left
};

const std::vector<glm::uvec3> indices {
        // back face
        {0u, 1u, 2u},
        {1u, 0u, 3u},
        // front face
        {4u, 5u, 6u},
        {6u, 7u, 4u},
        // left face
        {8u, 9u, 10u},
        {10u, 11u, 8u},
        // right face
        {12u, 13u, 14u},
        {13u, 12u, 15u},
        // bottom face
        {16u, 17u, 18u},
        {18u, 19u, 16u},
        // top face
        {20u, 21u, 22u},
        {21u, 20u, 23u}
};

Quadcopter::Quadcopter(const std::string &modelFilepath, const Parameters &params)
    : GameObject(), mode(Mode::ACRO), maxRoll(params.maxRoll), maxPitch(params.maxPitch),
    maxRollRate(params.maxRollRate), maxPitchRate(params.maxPitchRate), maxYawRate(params.maxYawRate),
    maxThrust(params.maxThrust),
    rollController(params.angle_kp, params.angle_ki, params.angle_kd),
    pitchController(params.angle_kp, params.angle_ki, params.angle_kd),
    motors{Motor(params.motorRotationSpeed2Thrust),
           Motor(params.motorRotationSpeed2Thrust),
           Motor(params.motorRotationSpeed2Thrust),
           Motor(params.motorRotationSpeed2Thrust)} {
    std::shared_ptr<Meshes> meshes(new Meshes{Mesh(std::make_shared<VertexArray>(positions,
                                                                                 normals,
                                                                                 textureCoords,
                                                                                 indices),
                                                   {Texture2D(glm::vec3{0.0f, 1.0f, 1.0f})},
                                                   {Texture2D(glm::vec3{1.0f, 1.0f, 1.0f})})});
    this->setMesh(std::move(meshes));

    // Create collision shape
    this->setCollisionShape(std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)));
    this->setUnscaledDimensions(glm::vec3(1.0f));

    auto k = params.controlRates2MotorRotationSpeed;
    this->controlRates2MotorRotationSpeeds = glm::mat4(k,  k,  k,  k,
                                                      -k,  k, -k,  k,
                                                      -k, -k,  k,  k,
                                                       k, -k, -k,  k);
    this->setMass(params.mass);
}

void Quadcopter::setMode(Mode mode) { this->mode = mode; }

void Quadcopter::onUpdate(std::chrono::duration<float> updateDuration) {
    if (this->mode == Mode::ANGLE) {
        // Calculate the roll and pitch rates to achieve target angles
        auto q = glm::quat_cast(this->getOrientation());
        auto roll = glm::roll(q);
        auto pitch = glm::pitch(q);

        this->controlRates[0] = this->rollController.computeCorrection(roll, this->targetRoll, updateDuration);
        this->controlRates[1] = this->pitchController.computeCorrection(pitch, this->targetPitch, updateDuration);

        this->controlRates[0] = clip(this->controlRates[0], -this->maxRollRate, this->maxRollRate);
        this->controlRates[1] = clip(this->controlRates[1], -this->maxPitchRate, this->maxPitchRate);

    }

    if (glm::length2(this->controlRates) < 0.001f) return;

    // Apply correction to motors
    auto rotationSpeeds = this->controlRates2MotorRotationSpeeds * this->controlRates;
    for (auto i = 0u; i < this->motors.size(); ++i) {
        motors[i].setRotationSpeed(rotationSpeeds[i]);
    }

    // Apply motor thrust
    auto halfDimensions = this->getScaledDimensions() / 2.0f;
    auto orientation = this->getOrientation();

    auto frontLeftForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[0].getThrust());
    auto frontRightForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[1].getThrust());
    auto backRightForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[2].getThrust());
    auto backLeftForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[3].getThrust());

    this->applyForce(frontLeftForce, {halfDimensions.x, halfDimensions.y, halfDimensions.z});
    this->applyForce(frontRightForce, {halfDimensions.x, -halfDimensions.y, halfDimensions.z});
    this->applyForce(backRightForce, {-halfDimensions.x, -halfDimensions.y, halfDimensions.z});
    this->applyForce(backLeftForce, {-halfDimensions.x, halfDimensions.y, halfDimensions.z});

    // Apply motor moments
    auto moment = glm::length(glm::vec3{halfDimensions.x, halfDimensions.y, 0.0f}) *
            (-this->motors[0].getThrust() +
              this->motors[1].getThrust() -
              this->motors[2].getThrust() +
              this->motors[3].getThrust());
    this->applyTorque(orientation * glm::vec3(0.0f, 0.0f, moment));
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

    this->controlRates[3] = input.y > 0.0f ? input.y * this->maxThrust : 0.0f;
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

    this->controlRates[2] = -input.x * this->maxYawRate;
}

Quadcopter::Motor::Motor(float rotationSpeed2Thrust) : rotationSpeed2Thrust(rotationSpeed2Thrust) {}
void Quadcopter::Motor::setRotationSpeed(float rotationSpeed) {this->rotationSpeed = rotationSpeed;}
float Quadcopter::Motor::getThrust() const {return this->rotationSpeed * this->rotationSpeed * this->rotationSpeed2Thrust;}

} // namespace age