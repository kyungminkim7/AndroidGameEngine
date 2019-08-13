#include <android_game_engine/Quadcopter.h>

#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

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
    motors{Motor(0, params.motorRotationSpeed2Thrust),
           Motor(1, params.motorRotationSpeed2Thrust),
           Motor(2, params.motorRotationSpeed2Thrust),
           Motor(3, params.motorRotationSpeed2Thrust)} {
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
    if (this->controlRates[3] < 0.001f) return;

    if (this->mode == Mode::ANGLE) {
        // Calculate the roll and pitch rates to achieve target angles
        glm::vec3 z_ref(0.0f, 0.0f, 1.0f);
        auto y_ref = glm::cross(z_ref, this->getOrientationX());
        auto x_ref = glm::cross(y_ref, z_ref);
        glm::mat3 R_world_ref(x_ref, y_ref, z_ref);

//        auto yaw = glm::eulerAngles(glm::toQuat(this->getOrientation()))[2];
//        auto R_world_ref = static_cast<glm::mat3>(glm::rotate(glm::mat4(1.0f),
//                                                              yaw,
//                                                              glm::vec3(0.0f, 0.0f, 1.0f)));

        auto R_ref_body = glm::transpose(R_world_ref) * this->getOrientation();
        auto rpy = glm::eulerAngles(glm::toQuat(R_ref_body));

        this->controlRates[0] = this->rollController.computeCorrection(rpy[0], this->targetRoll, updateDuration);
        this->controlRates[1] = this->pitchController.computeCorrection(rpy[1], this->targetPitch, updateDuration);

//        Log::info("Roll correction: " + std::to_string(glm::degrees(this->controlRates[0])));
//        Log::info("Pitch correction: " + std::to_string(glm::degrees(this->controlRates[1])));

        this->controlRates[0] = clip(this->controlRates[0], -this->maxRollRate, this->maxRollRate);
        this->controlRates[1] = clip(this->controlRates[1], -this->maxPitchRate, this->maxPitchRate);
    }

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
    auto halfDimensions = this->getScaledDimensions() / 2.0f;
    auto orientation = this->getOrientation();

    auto frontLeftForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[0].getThrust());
    auto frontRightForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[1].getThrust());
    auto backRightForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[2].getThrust());
    auto backLeftForce = orientation * glm::vec3(0.0f, 0.0f, this->motors[3].getThrust());

    this->applyForce(frontLeftForce, orientation * glm::vec3(halfDimensions.x, halfDimensions.y, halfDimensions.z));
    this->applyForce(frontRightForce, orientation * glm::vec3(halfDimensions.x, -halfDimensions.y, halfDimensions.z));
    this->applyForce(backRightForce, orientation * glm::vec3(-halfDimensions.x, -halfDimensions.y, halfDimensions.z));
    this->applyForce(backLeftForce, orientation * glm::vec3(-halfDimensions.x, halfDimensions.y, halfDimensions.z));

    // Apply motor moments
    auto armLength = glm::length(glm::vec3{halfDimensions.x, halfDimensions.y, 0.0f});

    for (auto i = 0u; i < this->motors.size(); ++i) {
        auto moment = armLength * this->motors[i].getThrust();
        this->applyTorque(orientation * glm::rotateZ(glm::vec3(moment, 0.0f, 0.0f),
                                                     glm::radians(-45.0f - 90.0f * i)));
    }

    auto yawMoment = armLength *
            (-this->motors[0].getThrust() +
              this->motors[1].getThrust() -
              this->motors[2].getThrust() +
              this->motors[3].getThrust());
    this->applyTorque(orientation * glm::vec3(0.0f, 0.0f, yawMoment));
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

Quadcopter::Motor::Motor(int id, float rotationSpeed2Thrust) : id(id), rotationSpeed2Thrust(rotationSpeed2Thrust) {}
void Quadcopter::Motor::setRotationSpeed(float rotationSpeed) {this->rotationSpeed = rotationSpeed;}
float Quadcopter::Motor::getThrust() const {
//    Log::info("Motor: " + std::to_string(id));
//    Log::info("Rpm: " + std::to_string(this->rotationSpeed * 30.0f / 3.14f));
//    Log::info("Thrust: " + std::to_string(this->rotationSpeed * this->rotationSpeed2Thrust) + "\n");
    return this->rotationSpeed * this->rotationSpeed2Thrust;
}


} // namespace age