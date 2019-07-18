#pragma once

#include "Widget.h"

namespace age {

class JoystickHandle;

///
/// \brief Widget implementing a joystick. The joystick provides the following outputs for each
///        position. Intermediate positions are interpolated to the limits:
///             - Center: (0.0f, 0.0f)
///             - Left: (-1.0f, 0.0f)
///             - Right: (1.0f, 0.0f)
///             - Up: (0.0f, 1.0f)
///             - Down: (0.0f, -1.0f
///
class Joystick : public Widget {
public:
    static std::shared_ptr<Joystick> New(Widget *parent=nullptr);
    
    void setDimensions(const glm::vec2 &dimensions) override;
    
    void setHandleSize(float handleSize);
    void setHandleColor(const glm::vec4 &color);
    
    void onTouchDown(const glm::vec2 &position) override;
    void onTouchMove(const glm::vec2 &position) override;
    void onTouchUp() override;

    virtual bool inBounds(const glm::vec2 &point) const;

protected:
    explicit Joystick(Widget *parent);

private:
    float getRadius() const;
    glm::vec2 getCenter() const;
    
    JoystickHandle *handle;
    float handleSize;
};

} // namespace age