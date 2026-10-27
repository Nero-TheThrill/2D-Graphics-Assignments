#pragma once
#include "course.hpp"
using namespace course;

struct Mesh
{
    GLuint vao=0, vbo=0, ebo=0;
    GLsizei indexCount=0;
    Mesh()=default;
    Mesh(const Mesh&)=delete;
    Mesh& operator=(const Mesh&)=delete;
    ~Mesh() {
        glDeleteVertexArrays(1,&vao);
        glDeleteBuffers(1,&vbo);
        glDeleteBuffers(1,&ebo);
    }
};

inline std::vector<Vertex> houseVertices()
{
    // Wall, roof, door, window. Separate vertices preserve each part's color.
    return {
        {-.5f,-.5f,.9f,.75f,.45f},{.5f,-.5f,.9f,.75f,.45f},
        {.5f,.25f,.9f,.75f,.45f},{-.5f,.25f,.9f,.75f,.45f},
        {-.6f,.25f,.85f,.2f,.2f},{.6f,.25f,.85f,.2f,.2f},{0,.75f,.85f,.2f,.2f},
        {-.12f,-.5f,.35f,.18f,.08f},{.12f,-.5f,.35f,.18f,.08f},
        {.12f,-.08f,.35f,.18f,.08f},{-.12f,-.08f,.35f,.18f,.08f},
        {.22f,-.02f,.2f,.65f,1},{.42f,-.02f,.2f,.65f,1},
        {.42f,.16f,.2f,.65f,1},{.22f,.16f,.2f,.65f,1}
    };
}
inline std::vector<unsigned> houseIndices()
{
    return {0,1,2,0,2,3, 4,5,6, 7,8,9,7,9,10, 11,12,13,11,13,14};
}
inline void createMesh(Mesh& mesh,const std::vector<Vertex>& vertices,const std::vector<unsigned>& indices)
{
    require(mesh.vao==0,"Create each immutable mesh only once");
    if(vertices.empty()||indices.empty()) return;
    for (auto index : indices) require(index < vertices.size(), "Invalid vertex index");
    glCreateBuffers(1,&mesh.vbo);
    glNamedBufferStorage(mesh.vbo,vertices.size()*sizeof(Vertex),vertices.data(),0);
    glCreateBuffers(1,&mesh.ebo);
    glNamedBufferStorage(mesh.ebo,indices.size()*sizeof(unsigned),indices.data(),0);
    glCreateVertexArrays(1,&mesh.vao);
    glVertexArrayVertexBuffer(mesh.vao,0,mesh.vbo,0,sizeof(Vertex));
    glVertexArrayElementBuffer(mesh.vao,mesh.ebo);
    glVertexArrayAttribFormat(mesh.vao,0,2,GL_FLOAT,GL_FALSE,offsetof(Vertex,x));
    glVertexArrayAttribFormat(mesh.vao,1,3,GL_FLOAT,GL_FALSE,offsetof(Vertex,r));
    for (GLuint location : {0u,1u}) {
        glVertexArrayAttribBinding(mesh.vao,location,0);
        glEnableVertexArrayAttrib(mesh.vao,location);
    }
    mesh.indexCount=GLsizei(indices.size());
}
inline void drawMesh(const Mesh& mesh)
{
    if(!mesh.vao) return;
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES,mesh.indexCount,GL_UNSIGNED_INT,nullptr);
}
