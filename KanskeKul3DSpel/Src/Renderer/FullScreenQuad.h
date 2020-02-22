#pragma once
#include "GL/glew.h"


struct Quad 
{
    const float quad[18] =
    {
         1, -1,  0,
        -1,  1,  0,
        -1, -1,  0,

         1,  1,  0,
        -1,  1,  0,
         1, -1,  0,
    };

    static Quad& get()
    {
        static Quad instance;
        return instance;
    }

    unsigned int getID() const { return id; }

private:

    Quad()
    {
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);

        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, 12 * 6, &quad[0], GL_STATIC_DRAW);
    }

    unsigned int id;
};