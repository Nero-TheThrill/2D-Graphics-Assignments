#version 450 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform mat3 uModel;
uniform mat3 uView;
uniform mat3 uCameraToNdc;

out vec3 vColor;

void main()
{
    // TODO A3.6: compose CameraToNdc * View * Model * localPosition.
    vec3 p = uModel * vec3(aPosition, 1.0); // A2 behavior until camera is implemented.

    gl_Position = vec4(p.xy, 0.0, 1.0);
    vColor = aColor;
}