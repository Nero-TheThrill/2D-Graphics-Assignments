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

    glCreateTextures(GL_TEXTURE_2D, 1, &texture.id);
    glTextureStorage2D(texture.id, 1, GL_RGB8, image.width, image.height);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTextureSubImage2D(texture.id, 0, 0, 0, image.width, image.height,
                        GL_RGB, GL_UNSIGNED_BYTE, image.rgb.data());
}

inline void textureSettings(const Texture& texture, bool linear, bool repeat)
{
    if (!texture.id) return;

    const GLint filter = linear ? GL_LINEAR : GL_NEAREST;
    const GLint wrap = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;
    glTextureParameteri(texture.id, GL_TEXTURE_MIN_FILTER, filter);
    glTextureParameteri(texture.id, GL_TEXTURE_MAG_FILTER, filter);
    glTextureParameteri(texture.id, GL_TEXTURE_WRAP_S, wrap);
    glTextureParameteri(texture.id, GL_TEXTURE_WRAP_T, wrap);
}

inline void createTexturedQuad(Mesh& mesh)
{
    createMesh(mesh, {
        {-.5f,-.5f,1,1,1,0,1}, {.5f,-.5f,1,1,1,1,1},
        {.5f,.5f,1,1,1,1,0}, {-.5f,.5f,1,1,1,0,0}
    }, {0,1,2,0,2,3});
    glVertexArrayAttribFormat(mesh.vao,2,2,GL_FLOAT,GL_FALSE,offsetof(Vertex,u));
    glVertexArrayAttribBinding(mesh.vao,2,0);
    glEnableVertexArrayAttrib(mesh.vao,2);
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
    require(columns>0 && rows>0 && width>0 && height>0, "Invalid atlas dimensions");
    require(tile>=0 && tile<columns*rows, "Invalid atlas tile");
    UvRect uv{{float(tile%columns)/columns, float(tile/columns)/rows},
              {1.0f/columns, 1.0f/rows}};
    return insetTile(uv,width,height);
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

    glBindTextureUnit(0,sprite.texture->id);
    glProgramUniform1i(program,glGetUniformLocation(program,"uTexture"),0);
    glProgramUniform1i(program,glGetUniformLocation(program,"uTextured"),1);
    glProgramUniform2f(program,glGetUniformLocation(program,"uUvOffset"),uv.offset.x,uv.offset.y);
    glProgramUniform2f(program,glGetUniformLocation(program,"uUvScale"),uv.scale.x,uv.scale.y);

    drawObject(sprite.object, program);
}
