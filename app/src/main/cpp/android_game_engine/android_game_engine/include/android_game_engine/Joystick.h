#pragma once

#include <forward_list>
#include <functional>

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
    using OnTouchDownCallback = std::function<void(const glm::vec2&)>;
    using OnTouchMoveCallback = std::function<void(const glm::vec2&)>;
    using OnTouchUpCallback = std::function<void()>;
    
    static std::shared_ptr<Joystick> New(Widget *parent=nullptr);
    
    void setDimensions(const glm::vec2 &dimensions) override;
    
    void setHandleSize(float handleSize);
    void setHandleColor(const glm::vec4 &color);
    
    void registerOnTouchDownCallback(OnTouchDownCallback callback);
    void registerOnTouchMoveCallback(OnTouchMoveCallback callback);
    void registerOnTouchUpCallback(OnTouchUpCallback callback);
    
    void onTouchDown(const glm::vec2 &position) override;
    void onTouchMove(const glm::vec2 &position) override;
    void onTouchUp() override;

protected:
    Joystick(Widget *parent);

private:
    float getRadius() const;
    glm::vec2 getCenter() const;
    
    bool inBounds(const glm::vec2 &point) const override;
    
    JoystickHandle *handle;
    float handleSize;
    
    std::forward_list<OnTouchDownCallback> onTouchDownCallbacks;
    std::forward_list<OnTouchMoveCallback> onTouchMoveCallbacks;
    std::forward_list<OnTouchUpCallback> onTouchUpCallbacks;
};

} // namespace age