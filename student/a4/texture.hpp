#pragma once

#include "camera.hpp"

struct Texture
{
    GLuint id = 0;
    int width = 0, height = 0;

    Texture() = default;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    ~Texture() { glDeleteTextures(1, &id); }
};

inline void createTexture(Texture& texture, const Image& image)
{
    texture.width = image.width;
    texture.height = image.height;

    // TODO A4.1: DSA create, allocate one mip level (RGB8), upload RGB/UNSIGNED_BYTE pixels.
    // glCreateTextures / glTextureStorage2D / glTextureSubImage2D.
    // No mipmaps in this assignment; set a non-mipmap minification filter below.
}

inline void textureSettings(const Texture& texture, bool linear, bool repeat)
{
    if (!texture.id) return;

    // TODO A4.2: set min/mag filters and S/T wrap with glTextureParameteri.
    // F toggles filtering. V toggles wrapping on the separate checker texture.
}

inline void createTexturedQuad(Mesh& mesh)
{
    // TODO A4.3: four white vertices with position and UV, six indices; use A1 createMesh.
    // Bottom vertices have v=1, top vertices v=0 to match the provided PPM loader.

    if (!mesh.vao) return;

    // TODO A4.4: connect UV attribute location 2 (two floats) to VBO binding 0.
    // Existing position/color attributes come from A1 createMesh.
}

struct UvRect
{
    Vec2 offset{0, 0};
    Vec2 scale{1, 1};
};

inline UvRect atlasRect(int frame, int columns, int rows, int width, int height)
{
    // TODO A4.5: row-major frame -> UV rectangle; inset half a texel to avoid adjacent-tile bleeding.
    return {}; // Whole atlas until frame selection is implemented.
}

struct Animation
{
    int frame = 0;
    int count = 8;
    float elapsed = 0;
    float interval = .15f;
    bool playing = true;
};

inline void updateAnimation(Animation& a, float dt)
{
    // TODO A4.6: retain fractional time and handle multiple frames in one update.
    // Use a while loop rather than resetting elapsed to zero.
}

struct Sprite
{
    Object object;
    const Texture* texture = nullptr;
    Animation animation;
};

inline void drawSprite(const Sprite& sprite, GLuint program, UvRect uv)
{
    if (!sprite.texture || !sprite.texture->id) return;

    // TODO A4.7: bind texture to unit 0, connect sampler, upload textured mode and UV rect.
    // Reuse drawObject for model upload and indexed drawing.

    drawObject(sprite.object, program);
}