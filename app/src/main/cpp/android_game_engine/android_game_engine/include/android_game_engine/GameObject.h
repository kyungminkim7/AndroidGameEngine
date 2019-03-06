#pragma once

#include <chrono>
#include <vector>

#include <glm/fwd.hpp>

#include "Model.h"

namespace age {

class Mesh;
class ShaderProgram;

///
/// \brief The GameObject class represents an object in the 3D virtual world.
///
class GameObject {
public:
//    GameObject();
    
    ///
    /// \brief GameObject Loads vertex and texture data and creates a model
    ///                   for the game object.
    /// \param modelFilepath Filepath to the model data.
    /// \exception ge::LoadError Failed to load mesh data from model file.
    /// \exception ge::LoadError Failed to load texture image from file.
    ///
//    GameObject(const std::string& modelFilepath);
    
//    GameObject(const std::vector<float> &positions,
//               const std::vector<float> &normals,
//               const std::vector<float> &textureCoords,
//               const std::vector<unsigned int> &indices,
//               const std::string &textureFilepath="");
    
    virtual ~GameObject() = default;
    
    ///
    /// \brief onUpdate Updates the game object's state.
    ///
    /// This should be called on every iteration of the game loop.
    /// The base implementation does nothing.
    ///
    /// \param updateDuration Elapsed time since the last frame.
    ///
    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    
    virtual void render(ShaderProgram *shader);
    
//    void setMesh(std::unique_ptr<Mesh> mesh);
    
    glm::mat4 getModelMatrix() const;
    
    ///
    /// \brief getNormalMatrix Returns the normal matrix.
    ///
    /// The returned normal matrix can be used to correct vertex normal vectors distorted through
    /// non-uniform scaling of the game object's model.
    ///
    /// The recommended use of this function is to use this to set a uniform value in an OpenGL
    /// vertex shader and then multiply by the vertex normal.
    ///
    /// Ex) correctedNormal = normalMatrix * vertexNormal;
    ///
    /// \return The normal matrix of the game object.
    ///
    glm::mat3 getNormalMatrix() const;
    
    glm::mat4 getViewMatrix() const;
    
    GameObject& setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() const;
    
    GameObject& setOrientation(const glm::mat3 &orientation);
    GameObject& setOrientation(const glm::vec3 &orientationX,
                               const glm::vec3 &orientationY,
                               const glm::vec3 &orientationZ);
    glm::mat3 getOrientation() const;
    glm::vec3 getOrientationX() const;
    glm::vec3 getOrientationY() const;
    glm::vec3 getOrientationZ() const;
    
    /// \name Look At
    ///
    /// Points the game object at a desired point or direction.
    ///
    /// If the new direction is linearly dependent with the game object's original normal,
    /// make sure to call GameObject::setNormal...() afterwards to properly set the new orientation.
    ///@{
    GameObject& setLookAtPoint(const glm::vec3 &lookAtPoint);
    GameObject& setLookAtDirection(const glm::vec3 &lookAtDirection);
    ///@}
    
    glm::vec3 getLookAtDirection() const;
    
    /// \name Normal
    ///
    /// Sets the game object's normal.
    ///
    /// If the new normal direction is linearly dependent with the game object's original
    /// look at direction, make sure to call GameObject::setLookAt...() afterwards to properly
    /// set the new orientation.
    ///@{
    GameObject& setNormalDirection(const glm::vec3 &normalDirection);
    ///@>
    
    glm::vec3 getNormalDirection() const;
    
    ///
    /// \brief rotate Rotates the game object about an axis in the world coordinate frame.
    /// \param angle_rad Angle to rotate game object by (rad).
    /// \param axis Axis (in world coordinate frame) to rotate game object about.
    /// \return The game object to allow chaining of multiple rotation/translation calls.
    ///
    GameObject& rotate(float angle_rad, const glm::vec3 &axis);
    
    ///
    /// \brief translate Translates the game object in the world coordinate frame.
    /// \param translation Amount to translate the game object by in the world coordinate frame.
    /// \return The game object to allow chaining of multiple rotation/translation calls.
    ///
    GameObject& translate(const glm::vec3 &translation);
    
    ///
    /// \brief translateInLocalFrame Translates the game object in the local coordinate frame.
    /// \param translation Amount to translate the game object by in the local coordinate frame.
    /// \return The game object to allow chaining of multiple rotation/translation calls.
    ///
    GameObject& translateInLocalFrame(const glm::vec3 &translation);
    
    GameObject& setScale(const glm::vec3 &scale);
    
    void setSpecularExponent(float specularExponent);

private:
//    using Meshes = std::vector<std::unique_ptr<Mesh>>;
    Model model;
    
//    std::shared_ptr<Meshes> meshes;
    float specularExponent = 64.0f;
};

inline glm::mat4 GameObject::getModelMatrix() const {return this->model.getModelMatrix();}
inline glm::mat3 GameObject::getNormalMatrix() const {return this->model.getNormalMatrix();}
inline glm::mat4 GameObject::getViewMatrix() const {return this->model.getViewMatrix();}

inline GameObject& GameObject::setPosition(const glm::vec3 &position) {
    this->model.setPosition(position);
    return *this;
}

inline glm::vec3 GameObject::getPosition() const {return this->model.getPosition();}

inline GameObject& GameObject::setOrientation(const glm::mat3& orientation) {
    this->model.setOrientation(orientation);
    return *this;
}

inline GameObject& GameObject::setOrientation(const glm::vec3 &orientationX,
                                              const glm::vec3 &orientationY,
                                              const glm::vec3 &orientationZ) {
    this->model.setOrientation(orientationX, orientationY, orientationZ);
    return *this;
}

inline glm::mat3 GameObject::getOrientation() const {return this->model.getOrientation();}
inline glm::vec3 GameObject::getOrientationX() const {return this->model.getOrientationX();}
inline glm::vec3 GameObject::getOrientationY() const {return this->model.getOrientationY();}
inline glm::vec3 GameObject::getOrientationZ() const {return this->model.getOrientationZ();}

inline GameObject& GameObject::setLookAtPoint(const glm::vec3 &lookAtPoint) {
    this->model.setLookAtPoint(lookAtPoint);
    return *this;
}

inline GameObject& GameObject::setLookAtDirection(const glm::vec3 &lookAtDirection) {
    this->model.setLookAtDirection(lookAtDirection);
    return *this;
}

inline glm::vec3 GameObject::getLookAtDirection() const {return this->model.getLookAtDirection();}

inline GameObject& GameObject::setNormalDirection(const glm::vec3 &normalDirection) {
    this->model.setNormalDirection(normalDirection);
    return *this;
}

inline glm::vec3 GameObject::getNormalDirection() const {return this->model.getNormalDirection();}

inline GameObject& GameObject::rotate(float angle_rad, const glm::vec3 &axis) {
    this->model.rotate(angle_rad, axis);
    return *this;
}

inline GameObject& GameObject::translate(const glm::vec3 &translation) {
    this->model.translate(translation);
    return *this;
}

inline GameObject& GameObject::translateInLocalFrame(const glm::vec3 &translation) {
    this->model.translateInLocalFrame(translation);
    return *this;
}

inline GameObject& GameObject::setScale(const glm::vec3& scale) {
    this->model.setScale(scale);
    return *this;
}

inline void GameObject::setSpecularExponent(float specularExponent) {
    this->specularExponent = specularExponent;
}

} // namespace age