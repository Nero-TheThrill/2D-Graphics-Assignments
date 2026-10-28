#version 450 core

layout(location=0) in vec2 aPosition;
layout(location=1) in vec3 aColor;

uniform mat3 uModel;

out vec3 vColor;

void main()
{
    // TODO A2.6: apply model transform to homogeneous local position.
    // Hint: Use GLSL vec3(aPosition, 1.0) and mat3 * vec3. A final component of 0 would discard translation.
    gl_Position=vec4(aPosition,0,1);
    vColor=aColor;
}
