#pragma once

#include <memory>
#include <string>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec3.hpp>

namespace age {

///
/// \brief Contains the pose data.
///
class Model
{
public:
    glm::mat4 getModelMatrix() const;
    
    ///
    /// \brief getNormalMatrix Returns the normal matrix.
    ///
    /// The returned normal matrix can be used to correct vertex normal vectors distorted through
    /// non-uniform scaling of the model.
    ///
    /// The recommended use of this function is to use this to set a uniform value in an OpenGL
    /// vertex shader and then multiply by the vertex normal.
    ///
    /// Ex) correctedNormal = normalMatrix * vertexNormal;
    ///
    /// \return The normal matrix of the model.
    ///
    glm::mat3 getNormalMatrix() const;
    
    glm::mat4 getViewMatrix() const;
    
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
    /// Points the model at a desired point or direction.
    ///
    /// If the new direction is linearly dependent with the model's original normal,
    /// make sure to call Model::setNormal...() afterwards to properly set the new orientation.
    ///@{
    void setLookAtPoint(const glm::vec3 &lookAtPoint);
    void setLookAtDirection(const glm::vec3 &lookAtDirection);
    ///@}
    
    glm::vec3 getLookAtDirection() const;
    
    /// \name Normal
    ///
    /// Sets the model's normal.
    ///
    /// If the new normal direction is linearly dependent with the model's original
    /// look at direction, make sure to call Model::setLookAt...() afterwards to properly
    /// set the new orientation.
    ///@{
    void setNormalDirection(const glm::vec3 &normalDirection);
    ///@>
    
    glm::vec3 getNormalDirection() const;
    
    ///
    /// \brief rotate Rotates the model about an axis in the world coordinate frame.
    /// \param angle_rad Angle to rotate model by (rad).
    /// \param axis Axis (in world coordinate frame) to rotate model about.
    ///
    void rotate(float angle_rad, const glm::vec3 &axis);
    
    ///
    /// \brief translate Translates the model in the world coordinate frame.
    /// \param translation Amount to translate the model by in the world coordinate frame.
    ///
    void translate(const glm::vec3 &translation);
    
    ///
    /// \brief translateInBodyFrame Translates the model in the local coordinate frame.
    /// \param translation Amount to translate the model by in the local coordinate frame.
    /// \return The model to allow chaining of multiple rotation/translation calls.
    ///
    void translateInLocalFrame(const glm::vec3 &translation);
    
    void setScale(const glm::vec3 &scale);
    glm::vec3 getScale() const;

private:
    glm::vec3 scale {1.0f};
    
    glm::vec3 position {0.0f};
    glm::mat3 orientation {1.0f};
    
    mutable glm::mat3 normalMatrix {1.0f};
    mutable bool normalMatrixIsValid = true;
};

inline glm::vec3 Model::getPosition() const {return this->position;}
inline glm::mat3 Model::getOrientation() const {return this->orientation;}
inline glm::vec3 Model::getOrientationX() const {return this->orientation[0];}
inline glm::vec3 Model::getOrientationY() const {return this->orientation[1];}
inline glm::vec3 Model::getOrientationZ() const {return this->orientation[2];}
inline glm::vec3 Model::getLookAtDirection() const {return this->orientation[0];}
inline glm::vec3 Model::getNormalDirection() const {return this->orientation[2];}
inline glm::vec3 Model::getScale() const {return this->scale;}

} // namespace age