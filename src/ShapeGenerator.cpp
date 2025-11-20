#include <ShapeGenerator.hpp>

#include <ezVertex.hpp>

#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

ShapeData ShapeGenerator::makeTriangle()
{
    ShapeData ret;
    ezVertex ezTri[] =
    {
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),

        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),

        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
    };

        ret.numVertices = NUM_ARRAY_ELEMENTS(ezTri);
        ret.vertices = new ezVertex[ret.numVertices];
        memcpy(ret.vertices, ezTri, sizeof(ezTri));

        GLushort indices[] = {0, 1, 2};
        ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
        ret.indices = new GLushort[ret.numIndices];
        memcpy(ret.indices, indices, sizeof(indices));

        return ret;

}
