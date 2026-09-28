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
    // TODO A2.1a: homogeneous 2D translation matrix, column-major storage.
    return Mat3::identity();
}

inline Mat3 rotation(float radians)
{
    // TODO A2.1b: counterclockwise 2D rotation matrix.
    return Mat3::identity();
}

inline Mat3 scaling(float x, float y)
{
    // TODO A2.1c: independent x/y scaling matrix.
    return Mat3::identity();
}

inline Mat3 modelMatrix(const Object& o, bool rotateTranslation = false)
{
    // TODO A2.2: TRS normally, RTS when rotateTranslation is true.
    return Mat3::identity();
}

inline std::vector<Object> makeObjects(const Mesh& mesh)
{
    // TODO A2.3: create three objects sharing mesh, with distinct position/angle/size.
    return {
        {&mesh, {0, 0}, 0, {1, 1}}
    }; // Prior assignment's single house.
}

inline void updateObject(Object& o, const App& app)
{
    // TODO A2.4: WASD translation, Q/E rotation, Z/X scale, R reset; use app.dt.
    // Only modify o; do not recreate the VBO or modify shared mesh vertices.
}

inline void drawObject(const Object& o, GLuint program, bool rotateTranslation = false)
{
    if (!o.mesh) return;

    // TODO A2.5: compute model matrix and upload uModel
    // (column-major, transpose GL_FALSE).
    // glProgramUniformMatrix3fv uploads to the named program.

    drawMesh(*o.mesh);
}