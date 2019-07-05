#pragma once

#include "Model.h"

#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

namespace age {

class ShaderProgram;

///
/// \brief The Light class represents a light.
///
class Light {
public:
    Light(const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);
    
    void setPosition(const glm::vec3 &position);
    glm::vec3 getPosition() const;
    
    virtual void render(ShaderProgram *shader) = 0;
    
    void setLookAtPoint(const glm::vec3 &lookAtPoint);
    
    void setLookAtDirection(const glm::vec3 &lookAtDirection);
    glm::vec3 getLookAtDirection() const;
    
    void setNormalDirection(const glm::vec3 &normalDirection);
    glm::vec3 getNormalDirection() const;
    
    void rotate(float angle_rad, const glm::vec3 &axis);
    
    void translate(const glm::vec3 &translation);
    void translateInLocalFrame(const glm::vec3 &translation);

    glm::mat4 getModelMatrix() const;
    glm::mat4 getViewMatrix() const;
    virtual glm::mat4 getProjectionMatrix() const = 0;
    
    void setAmbient(const glm::vec3 &ambient);
    void setDiffuse(const glm::vec3 &diffuse);
    void setSpecular(const glm::vec3 &specular);
    
    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
    
private:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    
    Model model;
};

inline void Light::setPosition(const glm::vec3 &position) {this->model.setPosition(position);}
inline glm::vec3 Light::getPosition() const {return this->model.getPosition();}

inline void Light::setLookAtPoint(const glm::vec3 &lookAtPoint) {this->model.setLookAtPoint(lookAtPoint);}
inline void Light::setLookAtDirection(const glm::vec3 &lookAtDirection) {this->model.setLookAtDirection(lookAtDirection);}
inline glm::vec3 Light::getLookAtDirection() const {return this->model.getLookAtDirection();}

inline void Light::setNormalDirection(const glm::vec3 &normalDirection) {this->model.setNormalDirection(normalDirection);}
inline glm::vec3 Light::getNormalDirection() const {return this->model.getNormalDirection();}

inline void Light::rotate(float angle_rad, const glm::vec3 &axis) {this->model.rotate(angle_rad, axis);}
inline void Light::translate(const glm::vec3 &translation) {this->model.translate(translation);}
inline void Light::translateInLocalFrame(const glm::vec3 &translation) {this->model.translateInLocalFrame(translation);}

inline glm::mat4 Light::getModelMatrix() const {return this->model.getModelMatrix();}
inline glm::mat4 Light::getViewMatrix() const {return this->model.getViewMatrix();}

inline void Light::setAmbient(const glm::vec3 &ambient) {this->ambient = ambient;}
inline void Light::setDiffuse(const glm::vec3 &diffuse) {this->diffuse = diffuse;}
inline void Light::setSpecular(const glm::vec3 &specular) {this->specular = specular;}

inline glm::vec3 Light::getAmbient() const {return this->ambient;}
inline glm::vec3 Light::getDiffuse() const {return this->diffuse;}
inline glm::vec3 Light::getSpecular() const {return this->specular;}

} // namespace age