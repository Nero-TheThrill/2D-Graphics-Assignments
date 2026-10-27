#pragma once
#include "objects.hpp"

struct Camera { Vec2 position{}; float angle=0; float height=6; };
inline Mat3 viewMatrix(const Camera& c) {
    return rotation(-c.angle)*translation(-c.position.x,-c.position.y);
}
inline Mat3 cameraToNdc(const Camera& c,float aspect) {
    return scaling(2/(c.height*aspect),2/c.height);
}
inline void updateCamera(Camera& c,const App& app) {
    c.position.x += 3*app.dt*(int(app.down(GLFW_KEY_RIGHT))-int(app.down(GLFW_KEY_LEFT)));
    c.position.y += 3*app.dt*(int(app.down(GLFW_KEY_UP))-int(app.down(GLFW_KEY_DOWN)));
    c.angle += app.dt*(int(app.down(GLFW_KEY_U))-int(app.down(GLFW_KEY_J)));
    c.height=std::clamp(c.height+4*app.dt*(int(app.down(GLFW_KEY_MINUS))-int(app.down(GLFW_KEY_EQUAL))),2.0f,20.0f);
    if(app.pressed(GLFW_KEY_C)) c=Camera{};
}
inline std::vector<Object> makeWorld(const Mesh& mesh) {
    std::vector<Object> world;
    // Leave the center clear for A4's texture examples.
    for(int y=-2;y<=2;++y)
        for(int x=-3;x<=3;++x) {
            if(y==0 && x>=-1 && x<=1) continue;
            world.push_back({&mesh,{x*2.2f,y*2.2f},0,{.65f,.65f}});
        }
    return world;
}
inline void uploadCamera(GLuint program,const Camera& c,int width,int height) {
    if(width<=0 || height<=0) return;
    const auto view=viewMatrix(c);
    const auto ndc=cameraToNdc(c,float(width)/height);
    glProgramUniformMatrix3fv(program,glGetUniformLocation(program,"uView"),1,GL_FALSE,view.m.data());
    glProgramUniformMatrix3fv(program,glGetUniformLocation(program,"uCameraToNdc"),1,GL_FALSE,ndc.m.data());
}
// Optional exercise: invert NDC mapping, then camera view.
inline Vec2 cursorToWorld(const Camera& c,double x,double y,int windowWidth,int windowHeight,float framebufferAspect) {
    if(windowWidth<=0 || windowHeight<=0) return c.position;
    Vec2 cameraPoint{float(2*x/windowWidth-1)*c.height*framebufferAspect/2,
                     float(1-2*y/windowHeight)*c.height/2};
    return (translation(c.position.x,c.position.y)*rotation(c.angle)).point(cameraPoint);
}
