#pragma once

#include "Widget.h"

namespace age {

class Joystick;

///
/// \brief Widget for joystick handle meant to be used by a Joystick Widget.
///
class JoystickHandle : public Widget {
public:
    static std::shared_ptr<JoystickHandle> New(Joystick *parent);
    
    void onTouchDown(const glm::vec2 &position) override;
    void onTouchMove(const glm::vec2 &position) override;
    void onTouchUp() override;
    
    float getRadius() const;
    
protected:
    JoystickHandle(Joystick *parent);

private:
    std::shared_ptr<Widget> handle;
};

inline float JoystickHandle::getRadius() const {return this->getDimensions().x * 0.5f;}

} // namespace age