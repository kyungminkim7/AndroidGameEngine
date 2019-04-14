#include <android_game_engine/Window.h>

namespace age {

std::shared_ptr<Window> Window::New() {
    return std::shared_ptr<Window>(new Window);
}

Window::Window() : Widget(nullptr) {}

void Window::onTouchDown(const glm::vec2 &position) {
    for (const auto& child : this->getChildren()) {
        if (child->inBounds(position)) return;
    }
    
    Widget::onTouchDown(position);
}

} // namespace age