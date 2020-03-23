#pragma once

#include <LinearMath/btIDebugDraw.h>

namespace age {

class ShaderProgram;

///
/// \brief Draws debugging objects for the Physics Engine
///
class PhysicsDebugDrawer : public btIDebugDraw {
public:
    explicit PhysicsDebugDrawer(ShaderProgram *shader);
    ~PhysicsDebugDrawer();

    PhysicsDebugDrawer(PhysicsDebugDrawer &&) noexcept = default;
    PhysicsDebugDrawer& operator=(PhysicsDebugDrawer &&) noexcept = default;
    
    void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
    void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,
                          int lifeTime, const btVector3 &color) override;
    
    void reportErrorWarning(const char* warningString) override;
    
    void draw3dText(const btVector3 &location, const char *textString) override;
    
    void setDebugMode(int debugMode) override;
    int getDebugMode() const override;
    
private:
    ShaderProgram *shader;
    unsigned int vao;
    unsigned int vbo;
    int debugMode;
};

inline int PhysicsDebugDrawer::getDebugMode() const {return this->debugMode;}

} // namespace age