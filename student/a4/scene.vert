#version 450 core
layout(location=0) in vec2 aPosition;
layout(location=1) in vec3 aColor;
layout(location=2) in vec2 aUv;
uniform mat3 uModel,uView,uCameraToNdc;
uniform vec2 uUvOffset,uUvScale;
out vec3 vColor;
out vec2 vUv;
void main(){
    vec3 p=uCameraToNdc*uView*uModel*vec3(aPosition,1);
    gl_Position=vec4(p.xy,0,1);vColor=aColor;
// TODO A4.8a: map quad UV to the selected atlas region.
    vUv=aUv;
}
