#include <android_game_engine/Quadcopter.h>

#include <limits>

#include <BulletCollision/CollisionShapes/btBoxShape.h>

#include <android_game_engine/VertexArray.h>

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

Quadcopter::Quadcopter(const std::string &modelFilepath)
    : GameObject(modelFilepath), maxMotorPercentDiff(0.05f), thrustMomentRatio(0.1f) {

//    std::shared_ptr<Meshes> meshes(new Meshes{Mesh(std::make_shared<VertexArray>(positions,
//                                                                                 normals,
//                                                                                 textureCoords,
//                                                                                 indices),
//                                                   {Texture2D(glm::vec3{0.0f, 1.0f, 1.0f})},
//                                                   {Texture2D(glm::vec3{1.0f, 1.0f, 1.0f})})});
//    this->setMesh(std::move(meshes));
//
//    // Create collision shape
//    this->setCollisionShape(std::make_unique<btBoxShape>(btVector3(0.5f, 0.5f, 0.5f)));
//    this->setUnscaledDimensions(glm::vec3(1.0f));
}

void Quadcopter::onUpdate(std::chrono::duration<float> updateDuration) {
    if (this->frontLeftThrust <= std::numeric_limits<float>::min() &&
        this->frontRightThrust <= std::numeric_limits<float>::min() &&
        this->backRightThrust <= std::numeric_limits<float>::min() &&
        this->backLeftThrust <= std::numeric_limits<float>::min()) return;
    
    auto frontLeftTorque = this->frontLeftThrust * this->thrustMomentRatio;
    auto frontRightTorque = this->frontRightThrust * this->thrustMomentRatio;
    auto backRightTorque = this->backRightThrust * this->thrustMomentRatio;
    auto backLeftTorque = this->backLeftThrust * this->thrustMomentRatio;
    
    auto halfDimensions = this->getScaledDimensions() / 2.0f;
    auto orientation = this->getOrientation();
    
    this->applyForce(orientation * glm::vec3(0.0f, 0.0f, this->frontLeftThrust),
                     {halfDimensions.x, halfDimensions.y, halfDimensions.z});
    this->applyForce(orientation * glm::vec3(0.0f, 0.0f, this->frontRightThrust),
                     {halfDimensions.x, -halfDimensions.y, halfDimensions.z});
    this->applyForce(orientation * glm::vec3(0.0f, 0.0f, this->backRightThrust),
                     {-halfDimensions.x, -halfDimensions.y, halfDimensions.z});
    this->applyForce(orientation * glm::vec3(0.0f, 0.0f, this->backLeftThrust),
                     {-halfDimensions.x, halfDimensions.y, halfDimensions.z});
    
    this->applyTorque(orientation * glm::vec3(0.0f, 0.0f, frontLeftTorque));
    this->applyTorque(orientation * glm::vec3(0.0f, 0.0f, -frontRightTorque));
    this->applyTorque(orientation * glm::vec3(0.0f, 0.0f, backRightTorque));
    this->applyTorque(orientation * glm::vec3(0.0f, 0.0f, -backLeftTorque));
}

void Quadcopter::onThrottleInput(const glm::vec2 &input) {
    if (input.y <= 0) {
        this->frontLeftThrust = this->frontRightThrust =
            this->backRightThrust = this->backLeftThrust = 0.0f;
        return;
    }
    
    auto thrust = this->maxMotorThrust * input.y;
    this->frontLeftThrust = this->frontRightThrust =
        this->backRightThrust = this->backLeftThrust =
            this->maxMotorThrust * input.y;
    
    this->frontLeftThrust = this->backLeftThrust =
        thrust * (1.0f - this->maxMotorPercentDiff * -input.x);
}

void Quadcopter::onControlInput(const glm::vec2 &input) {

}

void Quadcopter::setMaxMotorThrust(float thrust) {
    this->maxMotorThrust = thrust;
}

void Quadcopter::setThrustMomentRatio(float thrustMomentRatio) {
    this->thrustMomentRatio = thrustMomentRatio;
}

} // namespace age