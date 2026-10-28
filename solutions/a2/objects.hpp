#pragma once
#include "mesh.hpp"

struct Object {
    const Mesh* mesh=nullptr;
    Vec2 position{};
    float angle=0;
    Vec2 size{1,1};
};
inline Mat3 translation(float x,float y) {
    return {
        1, 0, x,
        0, 1, y,
        0, 0, 1};
}
inline Mat3 rotation(float radians) {
    const float c=std::cos(radians), s=std::sin(radians);
    return {
        c, -s, 0,
        s,  c, 0,
        0,  0, 1};
}
inline Mat3 scaling(float x,float y) {
    return {
        x, 0, 0,
        0, y, 0,
        0, 0, 1};
}
inline Mat3 modelMatrix(const Object& o,bool useSRT=false) {
    const auto t=translation(o.position.x,o.position.y);
    const auto r=rotation(o.angle);
    const auto s=scaling(o.size.x,o.size.y);
    return useSRT ? s*r*t : t*r*s;
}
inline std::vector<Object> makeObjects(const Mesh& mesh) {
    return {{&mesh,{-.65f,-.15f},-.15f,{.4f,.4f}},
            {&mesh,{0,.1f},0,{.5f,.5f}},
            {&mesh,{.65f,-.15f},.2f,{.35f,.35f}}};
}
inline void updateObject(Object& o,const App& app) {
    const float dt=app.dt;
    o.position.x += .5f*dt*(int(app.down(GLFW_KEY_D))-int(app.down(GLFW_KEY_A)));
    o.position.y += .5f*dt*(int(app.down(GLFW_KEY_W))-int(app.down(GLFW_KEY_S)));
    o.angle += dt*(int(app.down(GLFW_KEY_Q))-int(app.down(GLFW_KEY_E)));
    const float ds=.5f*dt*(int(app.down(GLFW_KEY_X))-int(app.down(GLFW_KEY_Z)));
    o.size.x=std::max(.05f,o.size.x+ds);
    o.size.y=std::max(.05f,o.size.y+ds);
    if(app.pressed(GLFW_KEY_R)) { o.position={}; o.angle=0; o.size={1,1}; }
}
inline void drawObject(const Object& o,GLuint program,bool useSRT=false) {
    if(!o.mesh) return;
    const auto model=modelMatrix(o,useSRT);
    glProgramUniformMatrix3fv(program,glGetUniformLocation(program,"uModel"),1,GL_FALSE,model.m.data());
    drawMesh(*o.mesh);
}
