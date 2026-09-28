#include "texture.hpp"
int main(int argc,char** argv){try{
    App app(argc,argv,"A4 - Textured world");
    Program program(std::string(SHADER_DIR)+"/scene.vert",std::string(SHADER_DIR)+"/scene.frag");
    Mesh house,quad;createMesh(house,houseVertices(),houseIndices());createTexturedQuad(quad);
    auto world=makeWorld(house);Camera camera;
    Texture atlas,checker;
    createTexture(atlas,loadPpm(std::string(COURSE_ROOT)+"/assets/atlas.ppm"));
    createTexture(checker,loadPpm(std::string(COURSE_ROOT)+"/assets/checker.ppm"));
    bool linear=false,repeat=true;
    textureSettings(atlas,linear,false);textureSettings(checker,linear,repeat);
    std::vector<Sprite> sprites;
// TODO A4.9: create three sprites sharing quad and atlas; different frames/speeds/positions.
    // Object owns transform state; Sprite adds a Texture pointer and Animation.
    Sprite wrapDemo{{&quad,{0,-1.5f},0,{2,1}},&checker,{}};
    while(app.begin()){
        glViewport(0,0,app.width,app.height);updateCamera(camera,app);
        glUseProgram(program.id);uploadCamera(program.id,camera,app.width,app.height);
// TODO A4.10: F/V toggles, Space pause, N step while paused, update each animation.
    // Use pressed for toggles; down would toggle every frame.
        // Color-only mode must be restored each frame after drawing textured sprites.
        glProgramUniform1i(program.id,glGetUniformLocation(program.id,"uTextured"),0);
        for(const auto& o:world)drawObject(o,program.id);
        for(const auto& s:sprites)drawSprite(s,program.id,atlasRect(s.animation.frame,4,2,atlas.width,atlas.height));
        drawSprite(wrapDemo,program.id,{{0,0},{3,3}});
        if(app.pressed(GLFW_KEY_F)||app.pressed(GLFW_KEY_V))
            std::printf("Filter: %s | checker wrap: %s\n",linear?"LINEAR":"NEAREST",repeat?"REPEAT":"CLAMP_TO_EDGE");
        app.end();
    }return 0;
}catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}}
