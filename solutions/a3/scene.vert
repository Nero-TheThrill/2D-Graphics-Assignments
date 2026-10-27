#version 450 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat3 uModel;
uniform mat3 uView;
uniform mat3 uCameraToNdc;

out vec3 vColor;

void main()
{
    // A3.6: local -> world -> camera -> NDC.
    vec3 p = uCameraToNdc * uView * uModel * vec3(aPosition, 1.0);

    gl_Position = vec4(p.xy, 0.0, 1.0);
    vColor = aColor;
}