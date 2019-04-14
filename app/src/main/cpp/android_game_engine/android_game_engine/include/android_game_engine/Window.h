#pragma once

#include "Widget.h"

namespace age {

class Window : public Widget {
public:
    
    static std::shared_ptr<Window> New();
    
    void onTouchDown(const glm::vec2 &position) override;

protected:
    Window();
};

} // namespace age