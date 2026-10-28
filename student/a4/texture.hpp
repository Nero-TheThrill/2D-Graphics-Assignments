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
    // Hint: Use target=GL_TEXTURE_2D, internalformat=GL_RGB8, format=GL_RGB, and type=GL_UNSIGNED_BYTE. Set RGB row alignment with glPixelStorei(GL_UNPACK_ALIGNMENT, ...) and restore the previous state.
    // glCreateTextures / glTextureStorage2D / glTextureSubImage2D.
    // No mipmaps in this assignment; set a non-mipmap minification filter below.
}

inline void textureSettings(const Texture& texture, bool linear, bool repeat)
{
    if (!texture.id) return;

    // TODO A4.2: set min/mag filters and S/T wrap with glTextureParameteri.
    // Hint: Set GL_TEXTURE_MIN_FILTER/MAG_FILTER to GL_NEAREST or GL_LINEAR, and GL_TEXTURE_WRAP_S/T to GL_REPEAT or GL_CLAMP_TO_EDGE.
    // F toggles filtering. V toggles wrapping on the separate checker texture.
}

inline void createTexturedQuad(Mesh& mesh)
{
    // TODO A4.3: four white vertices with position and UV, six indices; use A1 createMesh.
    // Hint: Use std::vector<Vertex>, std::vector<unsigned>, and createMesh. Vertex field order is x,y,r,g,b,u,v.
    // Bottom vertices have v=1, top vertices v=0 to match the provided PNG loader.

    if (!mesh.vao) return;

    // TODO A4.4: connect UV attribute location 2 (two floats) to VBO binding 0.
    // Hint: Use glVertexArrayAttribFormat, glVertexArrayAttribBinding, glEnableVertexArrayAttrib, and offsetof(Vertex, u).
    // Existing position/color attributes come from A1 createMesh.
}

struct UvRect
{
    Vec2 offset{0, 0};
    Vec2 scale{1, 1};
};

// Provided: keep LINEAR sampling inside a single atlas tile.
inline UvRect insetTile(UvRect uv, int width, int height)
{
    uv.offset.x += .5f / width;
    uv.offset.y += .5f / height;
    uv.scale.x -= 1.0f / width;
    uv.scale.y -= 1.0f / height;
    return uv;
}

inline UvRect atlasRect(int tile, int columns, int rows, int width, int height)
{
    // TODO A4.5: tile number -> row/column -> UV offset and scale.
    // Hint: Use integer / and % to find the row/column, and floating-point division for UV calculations. Finish with insetTile.
    // Return insetTile(uv, width, height) after computing the tile rectangle.
    return {}; // Whole atlas until tile selection is implemented.
}

struct Sprite
{
    Object object;
    const Texture* texture = nullptr;
    int tile = 0;
};

inline void drawSprite(const Sprite& sprite, GLuint program, UvRect uv)
{
    if (!sprite.texture || !sprite.texture->id) return;

    // TODO A4.6: bind texture to unit 0, connect sampler, upload textured mode and UV rect.
    // Hint: Use glBindTextureUnit, glGetUniformLocation, glProgramUniform1i (sampler and bool), and glProgramUniform2f (UV). The sampler value is a texture unit number, not a texture ID.
    // Reuse drawObject for model upload and indexed drawing.

    drawObject(sprite.object, program);
}
