#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include "headers.h"
#include "glslprogram.h"
#include "terrain.h"
#include "targa.h"

class GLObject
{
public:
    GLObject();

    bool init();
    void prepare(float dt);
    void render();
    void shutdown();

    void onResize(int width, int height);

private:
    float m_angle;
    Terrain m_terrain;
    GLSLProgram *m_glsl; // pointer to the GLSLProgram
    TargaImage m_grassTexture;
    GLuint m_grassTexID;
};

#endif
