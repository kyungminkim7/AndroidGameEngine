#include <android_game_engine/Joystick.h>

#include <cmath>

#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>

namespace age {

std::shared_ptr<Joystick> Joystick::New(Widget *parent) {
    std::shared_ptr<Joystick> joystick(new Joystick(parent));
    
    if (parent != nullptr) {
        parent->addChild(joystick);
    }
    
    return joystick;
}

Joystick::Joystick(age::Widget *parent) : Widget(parent), handleSize(0.4f) {
    this->setTexture("images/bordered_circle.png");
    this->handle = JoystickHandle::New(this);
}

void Joystick::setDimensions(const glm::vec2 &dimensions) {
    Widget::setDimensions(dimensions);
    this->setHandleSize(this->handleSize);
}

void Joystick::setHandleSize(float handleSize) {
    this->handleSize = handleSize;
    
    auto handleDimensions = this->getDimensions() * this->handleSize;
    this->handle->setDimensions(handleDimensions);
    this->handle->setPosition(this->getCenter() - handleDimensions * 0.5f);
}

void Joystick::setHandleColor(const glm::vec4 &color) {this->handle->setColor(color);}

void Joystick::onTouchDown(const glm::vec2 &position) {
    this->handle->setPosition(position - this->handle->getRadius());
}

void Joystick::onTouchMove(const glm::vec2 &position) {
    auto center = this->getCenter();
    auto v = position - center;
    auto angle = std::atan2(v.y, v.x);
    auto touchRadius = std::min(glm::distance(center, position),
            this->getRadius());
    
    this->handle->setPosition(center +
                              glm::rotate(glm::vec2{touchRadius, 0.0f}, angle) -
                              this->handle->getRadius());
}

void Joystick::onTouchUp() {
    this->handle->setPosition(this->getCenter() - this->handle->getRadius());
}

float Joystick::getRadius() const {return this->getDimensions().x * 0.5f;}

glm::vec2 Joystick::getCenter() const {
    auto radius = this->getRadius();
    return this->getPosition() + glm::vec2(radius, radius);
}

bool Joystick::inBounds(const glm::vec2 &point) const {
    auto radius = this->getRadius();
    return glm::distance2(point, this->getCenter()) < radius * radius;
}

} // namespace