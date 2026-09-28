#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace course {
inline void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}
constexpr float pi = 3.14159265358979323846f;
struct Vec2 { float x = 0, y = 0; };
struct Mat3 {
    // Column-major storage, column vectors. A * B applies B first.
    std::array<float, 9> m{};
    static Mat3 identity() { return {{1,0,0, 0,1,0, 0,0,1}}; }
    Mat3 operator*(const Mat3& b) const {
        Mat3 out;
        for (int c=0;c<3;++c) for(int r=0;r<3;++r) for(int k=0;k<3;++k)
            out.m[c*3+r] += m[k*3+r] * b.m[c*3+k];
        return out;
    }
    Vec2 point(Vec2 p) const {
        return {m[0]*p.x+m[3]*p.y+m[6], m[1]*p.x+m[4]*p.y+m[7]};
    }
};
// u/v are reserved in A1-A3 (zero); A4 connects them to an attribute.
struct Vertex { float x,y,r,g,b,u=0,v=0; };
inline std::string readText(const std::string& path) {
    std::ifstream in(path); require(bool(in), "Cannot open: "+path);
    std::ostringstream out; out << in.rdbuf(); return out.str();
}
inline GLuint compileShader(GLenum type,const std::string& source) {
    GLuint shader=glCreateShader(type); const char* s=source.c_str();
    glShaderSource(shader,1,&s,nullptr); glCompileShader(shader);
    GLint ok=0; glGetShaderiv(shader,GL_COMPILE_STATUS,&ok);
    if (!ok) {char log[4096]{};glGetShaderInfoLog(shader,sizeof log,nullptr,log);glDeleteShader(shader);throw std::runtime_error(log);}
    return shader;
}
inline GLuint compileProgram(const std::string& vs,const std::string& fs) {
    GLuint v=compileShader(GL_VERTEX_SHADER,vs),f=compileShader(GL_FRAGMENT_SHADER,fs);
    GLuint p=glCreateProgram(); glAttachShader(p,v);glAttachShader(p,f);glLinkProgram(p);
    glDeleteShader(v);glDeleteShader(f); GLint ok=0;glGetProgramiv(p,GL_LINK_STATUS,&ok);
    if(!ok){char log[4096]{};glGetProgramInfoLog(p,sizeof log,nullptr,log);glDeleteProgram(p);throw std::runtime_error(log);} return p;
}
struct Program {
    GLuint id=0;
    Program(const std::string& vs,const std::string& fs):id(compileProgram(readText(vs),readText(fs))){}
    ~Program(){glDeleteProgram(id);}
    Program(const Program&)=delete;
    Program& operator=(const Program&)=delete;
};
struct Image {int width=0,height=0;std::vector<unsigned char> rgb;};
inline Image loadPpm(const std::string& path) {
    std::ifstream in(path,std::ios::binary);require(bool(in),"Cannot open image: "+path);
    Image result;std::string magic;int maxv=0;
    in>>magic>>result.width>>result.height>>maxv;in.get();
    require(magic=="P6"&&result.width>0&&result.height>0&&maxv==255,"Expected bundled P6 PPM");
    result.rgb.resize(size_t(result.width)*result.height*3);
    in.read(reinterpret_cast<char*>(result.rgb.data()),std::streamsize(result.rgb.size()));
    require(bool(in),"Incomplete image");return result;
}
// PPM loader leaves first file scanline at v=0. A4 handles orientation in UVs.
struct App {
    GLFWwindow* window=nullptr;
    int width=960,height=640; float dt=0;
    GLuint indicatorProgram=0,indicatorVao=0;
    std::array<bool,GLFW_KEY_LAST+1> keys{},lastKeys{};
    bool leftClick=false,lastMouse=false;
    double previous=0;int frames=0,frameLimit=0;std::string capture;
    App(int argc,char** argv,const char* title) {
        for(int i=1;i<argc;++i){
            std::string arg=argv[i];
            if(arg=="--frames"&&i+1<argc) frameLimit=std::stoi(argv[++i]);
            else if(arg=="--capture"&&i+1<argc) capture=argv[++i];
        }
        glfwSetErrorCallback([](int,const char* s){std::fprintf(stderr,"GLFW: %s\n",s);});
        require(glfwInit()!=0,"GLFW initialization failed");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
        glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
        std::string caption=std::string(title)+" ["+COURSE_TRACK+"]";
        window=glfwCreateWindow(width,height,caption.c_str(),nullptr,nullptr);
        require(window!=nullptr,"OpenGL 4.5 Core context required. Update the GPU driver.");
        glfwMakeContextCurrent(window);glewExperimental=GL_TRUE;
        require(glewInit()==GLEW_OK,"GLEW initialization failed");
        while(glGetError()!=GL_NO_ERROR){}
        require(GLEW_VERSION_4_5!=0,"OpenGL 4.5 DSA is required");
        std::printf("Track: %s | OpenGL: %s\n",COURSE_TRACK,glGetString(GL_VERSION));
        glfwSwapInterval(1);
        // Supplied startup indicator, independent of students' mesh code.
        indicatorProgram=compileProgram(R"(#version 450 core
const vec2 p[3]=vec2[3](vec2(-.95,.78),vec2(-.79,.78),vec2(-.87,.95));
void main(){gl_Position=vec4(p[gl_VertexID],0,1);}
)",R"(#version 450 core
out vec4 color;void main(){color=vec4(1,.8,.2,1);}
)");
        glCreateVertexArrays(1,&indicatorVao);previous=glfwGetTime();
    }
    ~App(){glDeleteVertexArrays(1,&indicatorVao);glDeleteProgram(indicatorProgram);glfwDestroyWindow(window);glfwTerminate();}
    bool down(int key)const{return keys.at(key);}
    bool pressed(int key)const{return keys.at(key)&&!lastKeys.at(key);}
    bool begin() {
        if(glfwWindowShouldClose(window)||(frameLimit>0&&frames>=frameLimit)) return false;
        glfwPollEvents();lastKeys=keys;
        for(int i=GLFW_KEY_SPACE;i<=GLFW_KEY_LAST;++i)keys[i]=glfwGetKey(window,i)==GLFW_PRESS;
        bool mouse=glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS;
        leftClick=mouse&&!lastMouse;lastMouse=mouse;
        if(pressed(GLFW_KEY_ESCAPE))glfwSetWindowShouldClose(window,GLFW_TRUE);
        double now=glfwGetTime();dt=std::min(float(now-previous),.1f);previous=now;
        glfwGetFramebufferSize(window,&width,&height);
        // Viewport is supplied in A1/A2; A3/A4 explicitly update it in main.
        glClearColor(.075f,.095f,.14f,1);glClear(GL_COLOR_BUFFER_BIT);
        return true;
    }
    void end() {
        // Draw status in the full window, then restore the student viewport.
        GLint previousViewport[4];glGetIntegerv(GL_VIEWPORT,previousViewport);
        glViewport(0,0,width,height);glUseProgram(indicatorProgram);glBindVertexArray(indicatorVao);glDrawArrays(GL_TRIANGLES,0,3);
        if(!capture.empty() && (frameLimit==0||frames==frameLimit-1)){
            std::vector<unsigned char> rgb(size_t(width)*height*3);
            glPixelStorei(GL_PACK_ALIGNMENT,1);glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,rgb.data());
            std::ofstream out(capture,std::ios::binary);require(bool(out),"Cannot write capture");
            out<<"P6\n"<<width<<" "<<height<<"\n255\n";
            for(int y=height-1;y>=0;--y)out.write(reinterpret_cast<char*>(rgb.data()+size_t(y)*width*3),width*3);
        }
        glViewport(previousViewport[0],previousViewport[1],previousViewport[2],previousViewport[3]);
        GLenum error=glGetError();require(error==GL_NO_ERROR,"OpenGL error: "+std::to_string(error));
        glfwSwapBuffers(window);++frames;
    }
};
}
