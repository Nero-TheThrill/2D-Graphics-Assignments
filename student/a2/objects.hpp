#pragma once

#include "mesh.hpp"

// CPU scene data. Mesh is shared; each Object owns only its transform state.
struct Object
{
    const Mesh* mesh = nullptr;
    Vec2 position{};
    float angle = 0; // radians
    Vec2 size{1, 1};
};

inline Mat3 translation(float x, float y)
{
    // TODO A2.1a: homogeneous 2D translation matrix.
    // Hint: Initialize Mat3 with nine entries in braces, written row by row as on paper.
    return Mat3::identity();
}

inline Mat3 rotation(float radians)
{
    // TODO A2.1b: counterclockwise 2D rotation matrix.
    // Hint: Use std::cos(radians), std::sin(radians), and a Mat3 initializer. Write entries row by row.
    return Mat3::identity();
}

inline Mat3 scaling(float x, float y)
{
    // TODO A2.1c: independent x/y scaling matrix.
    // Hint: Initialize Mat3 with x/y scale factors and the homogeneous coordinate entry. No OpenGL calls are needed here.
    return Mat3::identity();
}

inline Mat3 modelMatrix(const Object& o, bool useSRT = false)
{
    // TODO A2.2: TRS normally, SRT when useSRT is true.
    // Hint: Reuse translation, rotation, scaling, and Mat3::operator*. A * B applies B first.
    return Mat3::identity();
}

inline std::vector<Object> makeObjects(const Mesh& mesh)
{
    // TODO A2.3: create three objects sharing mesh, with distinct position/angle/size.
    // Hint: Add Object{mesh pointer, position, angle, size} entries to std::vector<Object>. A2 positions are not measured in pixels.
    return {
        {&mesh, {0, 0}, 0, {1, 1}}
    }; // Prior assignment's single house.
}

inline void updateObject(Object& o, const App& app)
{
    // TODO A2.4: WASD translation, Q/E rotation, Z/X scale, R resets position/angle/size to (0,0)/0/(1,1); use app.dt.
    // Hint: Use app.down(GLFW_KEY_...) for held keys, app.pressed(...) for a new key press, and app.dt for frame time in seconds.
    // Only modify o; do not recreate the VBO or modify shared mesh vertices.
}

inline void drawObject(const Object& o, GLuint program, bool useSRT = false)
{
    if (!o.mesh) return;

    // TODO A2.5: compute model matrix and upload uModel
    // Hint: Use the GLint returned by glGetUniformLocation(program, "uModel") with glProgramUniformMatrix3fv(program, location, count, transpose, data). Access matrix data with m.data().
    // (column-major, transpose GL_FALSE).
    // glProgramUniformMatrix3fv uploads to the named program.

    drawMesh(*o.mesh);
}
