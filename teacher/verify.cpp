#include "texture.hpp"

void near(float actual,float expected) {
    require(std::abs(actual-expected)<0.0001f,"Numerical check failed");
}
void pointNear(Vec2 a,Vec2 b) { near(a.x,b.x); near(a.y,b.y); }
int main(int argc,char** argv) {
    try {
        Object object{nullptr,{4,5},pi/2,{2,3}};
        pointNear(modelMatrix(object).point({1,0}),{4,7});
        pointNear(modelMatrix(object,true).point({1,0}),{-5,6});
        Camera camera{{2,-3},.7f,6};
        pointNear(viewMatrix(camera).point(camera.position),{0,0});
        pointNear(cameraToNdc(camera,2).point({6,3}),{1,1});
        pointNear(cursorToWorld(camera,400,200,800,400,2),camera.position);
        const Vec2 cursorWorld=cursorToWorld(camera,600,100,800,400,2);
        pointNear((cameraToNdc(camera,2)*viewMatrix(camera)).point(cursorWorld),{.5f,.5f});
        const auto uv=atlasRect(5,4,2,256,128);
        near(uv.offset.x,.251953125f); near(uv.offset.y,.50390625f);
        near(uv.scale.x,.24609375f); near(uv.scale.y,.4921875f);

        App app(argc,argv,"Teacher verification");
        app.dt=.1f;
        Object moving{};
        app.keys[GLFW_KEY_D]=true;
        updateObject(moving,app);
        near(moving.position.x,.05f);
        app.keys.fill(false);
        app.keys[GLFW_KEY_R]=true;
        updateObject(moving,app);
        pointNear(moving.position,{0,0}); pointNear(moving.size,{1,1});
        app.keys.fill(false);
        app.keys[GLFW_KEY_EQUAL]=true;
        camera.height=2;
        updateCamera(camera,app); near(camera.height,2);
        app.keys.fill(false); app.keys[GLFW_KEY_MINUS]=true;
        camera.height=20;
        updateCamera(camera,app); near(camera.height,20);

        Texture tex;
        createTexture(tex,loadPng(std::string(COURSE_ROOT)+"/assets/checker.png"));
        for(bool linear : {false,true}) for(bool repeat : {false,true}) {
            textureSettings(tex,linear,repeat);
            GLint value=0;
            glGetTextureParameteriv(tex.id,GL_TEXTURE_MIN_FILTER,&value);
            require(value==(linear?GL_LINEAR:GL_NEAREST),"Wrong min filter");
            glGetTextureParameteriv(tex.id,GL_TEXTURE_MAG_FILTER,&value);
            require(value==(linear?GL_LINEAR:GL_NEAREST),"Wrong mag filter");
            for(GLenum axis : {GL_TEXTURE_WRAP_S,GL_TEXTURE_WRAP_T}) {
                glGetTextureParameteriv(tex.id,axis,&value);
                require(value==(repeat?GL_REPEAT:GL_CLAMP_TO_EDGE),"Wrong wrap");
            }
        }
        require(glGetError()==GL_NO_ERROR,"OpenGL verification error");
        std::puts("PASS: transforms, camera, cursor inversion, UV, input, zoom limits, texture settings");
        return 0;
    } catch(const std::exception& e) { std::fprintf(stderr,"%s\n",e.what()); return 1; }
}
