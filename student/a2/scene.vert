#version 450 core
layout(location=0) in vec2 aPosition;
layout(location=1) in vec3 aColor;
uniform mat3 uModel;
out vec3 vColor;
void main(){
// TODO A2.6: apply model transform to homogeneous local position.
    gl_Position=vec4(aPosition,0,1);
    vColor=aColor;
}
