#include "mesh.hpp"
int main(int argc,char** argv){try{
    App app(argc,argv,"A1 - DSA house");
    Program program(std::string(SHADER_DIR)+"/scene.vert",std::string(SHADER_DIR)+"/scene.frag");
    Mesh house;createMesh(house,houseVertices(),houseIndices());
    while(app.begin()){
        glViewport(0,0,app.width,app.height);glUseProgram(program.id);
        drawMesh(house);app.end();
    }
    return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
