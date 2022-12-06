// a mesh is a collection of vertices, faces and edges that define the shape of a 3d object
#ifndef _INCLUDED_MESH_H
#define _INCLUDED_MESH_H

#include <GL/glew.h>

class Mesh {
   public:
    Mesh();
    ~Mesh();

    void createMesh( GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices );
    void renderMesh();
    void clearMesh();

   private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};

#endif  // _INCLUDED_MESH_H