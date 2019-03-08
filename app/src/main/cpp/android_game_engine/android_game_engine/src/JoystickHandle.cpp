#include <android_game_engine/JoystickHandle.h>

#include <android_game_engine/Joystick.h>

namespace age {

std::shared_ptr<JoystickHandle> JoystickHandle::New(Joystick *parent) {
    std::shared_ptr<JoystickHandle> joystick(new JoystickHandle(parent));
    parent->addChild(joystick);
    return joystick;
}

JoystickHandle::JoystickHandle(age::Joystick *parent) : Widget(parent) {
    this->setTexture("images/filled_circle.png");
}

void JoystickHandle::onTouchDown(const glm::vec2 &position) {this->getParent()->onTouchDown(position);}
void JoystickHandle::onTouchMove(const glm::vec2 &position) {this->getParent()->onTouchMove(position);}
void JoystickHandle::onTouchUp() {this->getParent()->onTouchUp();}

} // namespace age