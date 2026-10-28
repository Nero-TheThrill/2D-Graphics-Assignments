#pragma once

#include "objects.hpp"

struct Camera
{
    Vec2 position{};
    float angle = 0;
    float height = 6;
};

inline Mat3 viewMatrix(const Camera& c)
{
    // TODO A3.1: inverse camera pose; inverse rotation after inverse translation.
    // Hint: Reuse A2 rotation, translation, and Mat3 multiplication to construct the inverse camera transform.
    return Mat3::identity();
}

inline Mat3 cameraToNdc(const Camera& c, float aspect)
{
    // TODO A3.2: map visible width = height * aspect and height to [-1, 1].
    // Hint: Reuse A2 scaling. Map each camera half-extent to 1 in NDC.
    return Mat3::identity();
}

inline void updateCamera(Camera& c, const App& app)
{
    // TODO A3.3: arrows pan, U/J rotate, =/- zoom, C resets; clamp height to [2, 20].
    // Hint: Use app.down, app.pressed, and app.dt. Limit height with std::clamp or std::min/std::max.
    // Pan in world axes. Camera motion does not edit Object positions.
}

inline std::vector<Object> makeWorld(const Mesh& mesh)
{
    // TODO A3.4: place houses both inside and outside the initial camera view.
    // Hint: Use std::vector<Object> with a shared mesh pointer. Determine the visible region from the initial camera height and aspect ratio.
    return makeObjects(mesh);
}

inline void uploadCamera(GLuint program, const Camera& c, int width, int height)
{
    if (width <= 0 || height <= 0) return;

    // TODO A3.5: compute aspect from framebuffer pixels;
    // Hint: Use viewMatrix, cameraToNdc, glGetUniformLocation, and glProgramUniformMatrix3fv. Use floating-point division for aspect and GL_FALSE for transpose.
    // upload uView and uCameraToNdc.
    // Matrices are column-major.
}

// Bonus helper contract:
// cursor uses GLFW window coordinates (top-left origin).
inline Vec2 cursorToWorld(const Camera& c, double x, double y, int windowWidth, int windowHeight, float framebufferAspect)
{
    // TODO A3.BONUS: cursor -> NDC -> camera -> world, including rotated cameras.
    // Hint: GLFW cursor coordinates increase downward in y. Convert to NDC, then undo camera projection and view. Reuse rotation, translation, and Mat3::point.
    return {};
}
