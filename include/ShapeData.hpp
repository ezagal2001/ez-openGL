#ifndef EZ_SHAPE_DATA
#define EZ_SHAPE_DATA
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ezVertex.hpp>

struct ShapeData
{
    ShapeData():
        vertices(0), numVertices(0),
        indices(0), numIndices(0) {}

    ezVertex *vertices;
    GLuint numVertices;
    GLushort *indices;
    GLuint numIndices;

    GLsizeiptr vertexBufferSize() const
    {
        return numVertices * sizeof(ezVertex);
    }

    GLsizeiptr indexBufferSize() const
    {
        return numIndices * sizeof(GLushort); 
    }

    void cleanup()
    {
        delete [] vertices;
        delete [] indices;
        numVertices = 0;
        numIndices = 0;
    }
};

#endif

