#pragma once

#include "Widget.h"

#include "JoystickHandle.h"

namespace age {

class Joystick : public Widget {
public:
    static std::shared_ptr<Joystick> New(Widget *parent=nullptr);
    
    void setDimensions(const glm::vec2 &dimensions) override;
    void setHandleSize(float handleSize);
    
    void setHandleColor(const glm::vec4 &color);
    
    void onTouchDown(const glm::vec2 &position) override;
    void onTouchMove(const glm::vec2 &position) override;
    void onTouchUp() override;
    

protected:
    Joystick(Widget *parent);

private:
    float getRadius() const;
    glm::vec2 getCenter() const;
    
    bool inBounds(const glm::vec2 &point) const override;
    
    std::shared_ptr<JoystickHandle> handle;
    float handleSize;
};

} // namespace age