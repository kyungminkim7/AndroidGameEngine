#pragma once

#include <chrono>

#include "Model.h"

namespace age {

///
/// \brief Represents a Camera.
///
class Camera {
public:
    ///
    /// \brief Camera constructor.
    ///
    /// \param fov_deg Camera field of view in degrees.
    /// \param aspectRatioWidthToHeight
    /// \param nearPlane Distance to camera near plane.
    /// \param farPlane Distance to camera far plane.
    ///
    Camera(float fov_deg, float aspectRatioWidthToHeight, float nearPlane, float farPlane);
    
    virtual ~Camera() = default;
    
    void setFov_deg(float fov_deg);
    float getFov_deg() const;
    
    void setAspectRatioWidthToHeight(float aspectRatioWidthToHeight);
    
    ///
    /// \brief onUpdate Updates the game object's state.
    ///
    /// This should be called on every iteration of the game loop.
    /// The base implementation does nothing.
    ///
    /// \param updateDuration Elapsed time since the last frame.
    ///
    virtual void onUpdate(std::chrono::duration<float> updateDuration);
    
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
    
    glm::mat4 getProjectionMatrix() const;
    
    void setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() const;
    
    void setOrientation(const glm::mat3 &orientation);
    void setOrientation(const glm::vec3 &orientationX,
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
    void setLookAtPoint(const glm::vec3 &lookAtPoint);
    void setLookAtDirection(const glm::vec3 &lookAtDirection);
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
    void setNormalDirection(const glm::vec3 &normalDirection);
    ///@>
    
    glm::vec3 getNormalDirection() const;
    
    ///
    /// \brief rotate Rotates the game object about an axis in the world coordinate frame.
    /// \param angle_rad Angle to rotate game object by (rad).
    /// \param axis Axis (in world coordinate frame) to rotate game object about.
    ///
    void rotate(float angle_rad, const glm::vec3 &axis);
    
    ///
    /// \brief translate Translates the game object in the world coordinate frame.
    /// \param translation Amount to translate the game object by in the world coordinate frame.
    ///
    void translate(const glm::vec3 &translation);
    
    ///
    /// \brief translateInLocalFrame Translates the game object in the local coordinate frame.
    /// \param translation Amount to translate the game object by in the local coordinate frame.
    ///
    void translateInLocalFrame(const glm::vec3 &translation);
    
    void setScale(const glm::vec3 &scale);
    
    void setSpecularExponent(float specularExponent);

private:
    void updateProjectionMatrix();
    
    float fov_rad;
    float aspectRatioWidthToHeight;
    float nearPlane; ///< m
    float farPlane;  ///< m
    
    Model model;
    glm::mat4 projectionMatrix {1.0f};
};

inline glm::mat4 Camera::getModelMatrix() const {return this->model.getModelMatrix();}
inline glm::mat3 Camera::getNormalMatrix() const {return this->model.getNormalMatrix();}
inline glm::mat4 Camera::getViewMatrix() const {return this->model.getViewMatrix();}
inline glm::mat4 Camera::getProjectionMatrix() const {return this->projectionMatrix;}
inline void Camera::setPosition(const glm::vec3 &position) {this->model.setPosition(position);}
inline glm::vec3 Camera::getPosition() const {return this->model.getPosition();}
inline void Camera::setOrientation(const glm::mat3& orientation) {this->model.setOrientation(orientation);}
inline void Camera::setOrientation(const glm::vec3 &orientationX,
                                   const glm::vec3 &orientationY,
                                   const glm::vec3 &orientationZ) {
    this->model.setOrientation(orientationX, orientationY, orientationZ);
}
inline glm::mat3 Camera::getOrientation() const {return this->model.getOrientation();}
inline glm::vec3 Camera::getOrientationX() const {return this->model.getOrientationX();}
inline glm::vec3 Camera::getOrientationY() const {return this->model.getOrientationY();}
inline glm::vec3 Camera::getOrientationZ() const {return this->model.getOrientationZ();}
inline glm::vec3 Camera::getLookAtDirection() const {return this->model.getLookAtDirection();}
inline glm::vec3 Camera::getNormalDirection() const {return this->model.getNormalDirection();}
inline void Camera::setLookAtPoint(const glm::vec3 &lookAtPoint) {this->model.setLookAtPoint(lookAtPoint);}
inline void Camera::setLookAtDirection(const glm::vec3 &lookAtDirection) {this->model.setLookAtDirection(lookAtDirection);}
inline void Camera::setNormalDirection(const glm::vec3 &normalDirection) {this->model.setNormalDirection(normalDirection);}
inline void Camera::rotate(float angle_rad, const glm::vec3 &axis) {this->model.rotate(angle_rad, axis);}
inline void Camera::translate(const glm::vec3 &translation) {this->model.translate(translation);}
inline void Camera::translateInLocalFrame(const glm::vec3 &translation) {this->model.translateInLocalFrame(translation);}

} // namespace age