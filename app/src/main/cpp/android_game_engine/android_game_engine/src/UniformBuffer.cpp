#include <android_game_engine/UniformBuffer.h>

#include <deque>

#include <GLES3/gl32.h>

namespace {

///
/// The Pool class generates sequential elements.
/// Elements can be recycled back into the pool.
///
template<typename T>
class Pool {
public:
    Pool(std::initializer_list<T> init) : pool(init) {}

    T popFront() {
        T element = this->pool.front();
        this->pool.pop_front();

        if (this->pool.empty()) {
            this->pool.push_front(element + 1);
        }

        return element;
    }

    void pushFront(const T &element) {
        this->pool.push_front(element);
    }

private:
    std::deque<T> pool;
};

Pool<unsigned int> bindingPointPool {0, 1};

} // namespace

namespace age {

UniformBuffer::UniformBuffer(const std::string &uniformBlockName, unsigned int size_bytes)
        : uniformBlockName(uniformBlockName), bindingPoint(bindingPointPool.popFront()) {
    glGenBuffers(1, &this->ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferData(GL_UNIFORM_BUFFER, size_bytes, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingPoint, this->ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer() {
    glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingPoint, 0);
    bindingPointPool.pushFront(this->bindingPoint);
    glDeleteBuffers(1, &this->ubo);
}

std::string UniformBuffer::getUniformBlockName() const {return this->uniformBlockName;}
unsigned int UniformBuffer::getBindingPoint() const {return this->bindingPoint;}

void UniformBuffer::bufferSubData(unsigned int offset_bytes, unsigned int size_bytes, const void *data) {
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset_bytes, size_bytes, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

} // namespace age