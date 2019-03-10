#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "Model.h"
#include "Texture2D.h"
#include "TouchEvent.h"

namespace age {

class ShaderProgram;

///
/// \brief Base class for building GUI Widgets. All GUI widgets are described in the following
///        screen coordinate system:
///             - Upper left: (0, 0)
///             - Upper right: (dimensions.x, 0)
///             - Lower left: (0, dimensions.y)
///             - Lower right: (dimensions.x, dimensions.y)
///
class Widget {
public:
    static std::shared_ptr<Widget> New(Widget *parent=nullptr);
    virtual ~Widget();
    
    void addChild(std::shared_ptr<Widget> child);
    
    void setTexture(const std::string &imageFilepath);
    void setColor(const glm::vec4 &color);
    
    ///
    /// Sets the upper left position and dimensions of this widget relative to its parent
    /// in screen coordinates.
    /// \param upperLeftPosition
    /// \param dimensions
    void setGeometry(const glm::vec2 &upperLeftPosition, const glm::vec2 &dimensions);
    
    void setRelativePosition(const glm::vec2 &upperLeftPosition);
    glm::vec2 getRelativePosition() const;
    
    void setPosition(const glm::vec2 &upperLeftPosition);
    glm::vec2 getPosition() const;
    
    virtual void setDimensions(const glm::vec2 &dimensions);
    glm::vec2 getDimensions() const;
    
    virtual void render(ShaderProgram *shader);
    
    void onTouchDownEvent(const TouchEvent &event);
    void onTouchMoveEvent(const TouchEvent &event);
    void onTouchUpEvent(const TouchEvent &event);
    
    virtual void onTouchDown(const glm::vec2 &position);
    virtual void onTouchMove(const glm::vec2 &position);
    virtual void onTouchUp();

protected:
    Widget(Widget *parent);
    Widget* getParent();

private:
    virtual bool inBounds(const glm::vec2 &point) const;
    
    Widget *parent;
    std::vector<std::shared_ptr<Widget>> children;
    
    Model model;
    glm::vec2 position; //< absolute position in window
    glm::vec2 dimensions;
    
    std::shared_ptr<unsigned int> vbo;
    std::unique_ptr<Texture2D> texture = nullptr;
    glm::vec4 color {0.0f, 0.0f, 0.0f, 1.0f};
};

inline Widget* Widget::getParent() {return this->parent;}

inline glm::vec2 Widget::getRelativePosition() const {
    return this->position - this->parent->getPosition();
}

inline glm::vec2 Widget::getPosition() const {return this->position;}
inline glm::vec2 Widget::getDimensions() const {return this->dimensions;}

} // namespace age