#include <android_game_engine/Joystick.h>

#include <cmath>

#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <android_game_engine/JoystickHandle.h>
#include <android_game_engine/Log.h>
#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/Utilities.h>

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
    this->handle = JoystickHandle::New(this).get();
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
    
    auto output = (position - this->getCenter()) /
            (this->getDimensions() - this->handle->getDimensions()) * 2.0f;
    output.y *= -1.0f;

    Widget::onTouchDown(output);
}

void Joystick::onTouchMove(const glm::vec2 &position) {
    auto center = this->getCenter();
    auto halfDimensions = this->getDimensions() * 0.5f;
    auto handleHalfDimensions = this->handle->getDimensions() * 0.5f;
    auto lowerBound = center - halfDimensions + handleHalfDimensions;
    auto upperBound = center + halfDimensions - handleHalfDimensions;
    glm::vec2 handlePosition(
            clip(position.x, lowerBound.x, upperBound.x),
            clip(position.y, lowerBound.y, upperBound.y));

    this->handle->setPosition(handlePosition - this->handle->getRadius());
    
    auto output = (handlePosition - center) / (halfDimensions - handleHalfDimensions);
    output.y *= -1.0f;

    Widget::onTouchMove(output);
}

void Joystick::onTouchUp() {
    this->handle->setPosition(this->getCenter() - this->handle->getRadius());
    
    Widget::onTouchUp();
}

bool Joystick::inBounds(const glm::vec2 &point) const {
    auto center = this->getCenter();
    auto halfDimensions = this->getDimensions() * 0.5f;
    auto handleHalfDimensions = this->handle->getDimensions() * 0.5f;
    auto lowerBound = center - halfDimensions + handleHalfDimensions;
    auto upperBound = center + halfDimensions - handleHalfDimensions;
    return point.x >= lowerBound.x && point.y >= lowerBound.y &&
        point.x <= upperBound.x && point.y <= upperBound.y;
}

float Joystick::getRadius() const {return this->getDimensions().x * 0.5f;}

glm::vec2 Joystick::getCenter() const {
    auto radius = this->getRadius();
    return this->getPosition() + glm::vec2(radius, radius);
}

} // namespace