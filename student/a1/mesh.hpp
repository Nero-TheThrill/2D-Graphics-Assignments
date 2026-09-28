#pragma once
#include "course.hpp"
using namespace course;

struct Mesh 
{
    GLuint vao=0,vbo=0,ebo=0;
    GLsizei indexCount=0;
    Mesh()=default;
    Mesh(const Mesh&)=delete; 
    Mesh& operator=(const Mesh&)=delete;
    ~Mesh()
    {
        glDeleteVertexArrays(1,&vao);
        glDeleteBuffers(1,&vbo);
        glDeleteBuffers(1,&ebo);
    }
};

inline std::vector<Vertex> houseVertices()
{
    // TODO A1.1: build wall, roof, door, and window vertices (x,y,r,g,b).
    return {{-.55f,.25f,1,.3f,.3f},{.55f,.25f,1,.3f,.3f},{0,.7f,1,.3f,.3f}};
}
inline std::vector<unsigned> houseIndices()
{
    // TODO A1.2: add triangle indices for all four house parts.
    return {0,1,2};
}
inline void createMesh(Mesh& mesh,const std::vector<Vertex>& vertices,const std::vector<unsigned>& indices)
{
    require(mesh.vao==0,"Create each immutable mesh only once");
    if(vertices.empty()||indices.empty())
        return;
    // TODO A1.3: glCreateBuffers + glNamedBufferStorage for VBO/EBO; byte sizes, flags=0.
    // Create names in mesh.vbo / mesh.ebo and upload the supplied arrays.
    // TODO A1.4: create VAO, attach VBO to binding 0, attach EBO.
    // Binding index 0; offset 0; stride sizeof(Vertex).
    // TODO A1.5: configure position (location 0, 2 floats) and color (location 1, 3 floats).
    // Both attributes use VBO binding index 0. u/v are not enabled yet.
    mesh.indexCount=GLsizei(indices.size());
}
inline void drawMesh(const Mesh& mesh)
{
    if(!mesh.vao)
        return; // Keeps an unfinished starter runnable.
    // TODO A1.6: bind VAO and issue the indexed draw call.
    // Count means number of indices, not number of triangles.
}
