#version 450 core

layout(location=0) in vec2 aPosition;
layout(location=1) in vec3 aColor;

uniform mat3 uModel;

out vec3 vColor;

void main()
{
    // A2.6: local -> world.
    vec3 p = uModel * vec3(aPosition, 1.0);
    gl_Position=vec4(p.xy,0,1);
    vColor=aColor;
}
