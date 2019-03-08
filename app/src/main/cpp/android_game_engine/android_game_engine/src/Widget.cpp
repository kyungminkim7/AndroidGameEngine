#include <android_game_engine/Widget.h>

#include <algorithm>
#include <array>
#include <forward_list>
#include <iterator>
#include <unordered_map>

#include <GLES2/gl2.h>

#include <android_game_engine/ManagerWindowing.h>
#include <android_game_engine/ShaderProgram.h>
#include <android_game_engine/Utilities.h>

namespace {

std::array<float, 24> vertices {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};

std::weak_ptr<unsigned int> vboCache;
std::unordered_map<int, age::Widget*> touchedWidgets;

} // namespace

namespace age {

std::shared_ptr<Widget> Widget::New(Widget *parent) {
    std::shared_ptr<Widget> widget(new Widget(parent));
    
    if (parent != nullptr) {
        parent->addChild(widget);
    }
    
    return widget;
}

Widget::Widget(Widget *parent) : parent(parent) {
    this->vbo = vboCache.lock();
    if (this->vbo) return;
    
    // Clean up vbo on GPU after last Widget is deleted
    auto vboDeleter = [](auto vbo) {
        glDeleteBuffers(1, vbo);
        delete vbo;
    };
    this->vbo = std::shared_ptr<unsigned int>(new unsigned int, vboDeleter);
    
    // Load vertex data
    glGenBuffers(1, this->vbo.get());
    glBindBuffer(GL_ARRAY_BUFFER, *this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
            GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    vboCache = this->vbo;
}

Widget::~Widget() {
    for (auto& child : this->children) {
        child->parent = nullptr;
    }
}

void Widget::addChild(std::shared_ptr<Widget> child) {
    this->children.push_back(std::move(child));
}

void Widget::setTexture(const std::string &imageFilepath) {
    this->texture = std::make_unique<Texture2D>(imageFilepath);
}

void Widget::setColor(const glm::vec4 &color) {this->color = color;}

void Widget::setGeometry(const glm::vec2 &upperLeftPosition, const glm::vec2 &dimensions) {
    this->setRelativePosition(upperLeftPosition);
    this->setDimensions(dimensions);
}

void Widget::setRelativePosition(const glm::vec2 &upperLeftPosition) {
    if (this->parent == nullptr) {
        this->setPosition(upperLeftPosition);
    } else {
        this->setPosition(this->parent->getPosition() + upperLeftPosition);
    }
}

void Widget::setPosition(const glm::vec2 &upperLeftPosition) {
    for (auto &child : children) {
        child->setPosition(child->getPosition() - this->position + upperLeftPosition);
    }
    
    this->model.setPosition({upperLeftPosition.x,
                             ManagerWindowing::getWindowHeight() - upperLeftPosition.y, 0.0f});
    this->position = upperLeftPosition;
}

void Widget::setDimensions(const glm::vec2 &dimensions) {
    this->dimensions = dimensions;
    this->model.setScale({dimensions.x, -dimensions.y, 1.0f});
}

void Widget::render(ShaderProgram *shader) {
    if (this->parent != nullptr) {
        shader->setUniform("model", this->model.getModelMatrix());
    
        // Bind vbo
        constexpr static auto stride = 4 * sizeof(float);
        glBindBuffer(GL_ARRAY_BUFFER, *this->vbo);
        shader->setVertexAttribPointer("aPos", 2, GL_FLOAT, GL_FALSE, stride,
                                       reinterpret_cast<GLvoid*>(0));
        shader->setVertexAttribPointer("aTexCoord", 2, GL_FLOAT, GL_FALSE, stride,
                                       reinterpret_cast<GLvoid*>(2 * sizeof(float)));
    
        // Bind texture
        shader->setUniform("color", this->color);
    
        shader->setUniform("texture0", 0);
        glActiveTexture(GL_TEXTURE0);
        this->texture->bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    for (auto& child : this->children) {
        child->render(shader);
    }
}

void Widget::onTouchDownEvent(const age::TouchEvent &event) {
    for (const auto& e : event) {
        if (this->inBounds(e.second)) {
            touchedWidgets[e.first] = this;
            this->onTouchDown(e.second);
            
            for (auto& child : this->children) {
                child->onTouchDownEvent({e});
            }
        }
    }
}

void Widget::onTouchMoveEvent(const age::TouchEvent &event) {
    // Extract the touched and event ids
    auto keyExtractor = [](const auto& item){return item.first;};
    
    std::vector<int> touchedIds;
    touchedIds.reserve(touchedWidgets.size());
    std::transform(touchedWidgets.begin(), touchedWidgets.end(),
            std::back_inserter(touchedIds), keyExtractor);
    
    std::vector<int> eventIds;
    eventIds.reserve(event.size());
    std::transform(event.begin(), event.end(),
            std::back_inserter(eventIds), keyExtractor);
    
    // Find which widgets have been released
    std::sort(touchedIds.begin(), touchedIds.end());
    std::sort(eventIds.begin(), eventIds.end());
    
    std::forward_list<int> touchUpIds;
    std::set_difference(touchedIds.begin(), touchedIds.end(),
                        eventIds.begin(), eventIds.end(),
                        std::front_inserter(touchUpIds));
    
    for (auto id : touchUpIds) {
        touchedWidgets[id]->onTouchUp();
        touchedWidgets.erase(id);
    }
    
    for (const auto& e : event) {
        auto w = touchedWidgets.find(e.first);
        if (w != touchedWidgets.end()) {
            // Find the corresponding widget the move event belongs to
            w->second->onTouchMove(e.second);
        } else {
            // A new widget has been touched
            this->onTouchDownEvent({e});
        }
    }
}

void Widget::onTouchUpEvent(const age::TouchEvent &event) {
    for (const auto& e : event) {
        auto w = touchedWidgets.find(e.first);
        if (w != touchedWidgets.end()) {
            w->second->onTouchUp();
        }
    }
    touchedWidgets.clear();
}

void Widget::onTouchDown(const glm::vec2 &position) {}
void Widget::onTouchMove(const glm::vec2 &position) {}
void Widget::onTouchUp() {}

bool Widget::inBounds(const glm::vec2 &point) const {
    return (point.x > this->position.x) &&
           (point.x < this->position.x + this->dimensions.x) &&
           (point.y > this->position.y) &&
           (point.y < this->position.y + this->dimensions.y);
}

} // namespace age
