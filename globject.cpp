#ifdef _WIN32
#include <windows.h>
#endif

#include "globject.h"

GLObject::GLObject()
{
    m_glsl = new GLSLProgram("data/glsl/basic.vert",
                             "data/glsl/basic.frag");
    m_angle = 0.0f;
}

bool GLObject::init()
{
    std::cerr << "WTF" << std::endl;
    if (!m_glsl->initialize()) {
        std::cerr << "GLSL Program fails at initialization." << std::endl;
        return false;
    }

    std::cout << "Succeeded in loading glsl initialization" << std::endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glClearColor(0.0f, 0.5f, 0.9f, 0.5f);

    if (!m_terrain.loadHeightmap("data/heightmap.raw", 65))
    {
        std::cerr << "Cannot load heightmap" << std::endl;
        return false;
    }

    std::cout << "Succeeded in loading heightmap" << std::endl;

    // bind the attribute locations
    m_glsl->bindAttrib(0, "a_Vertex");
    m_glsl->bindAttrib(1, "a_Color");
    m_glsl->bindAttrib(2, "a_TexCoord0");

    // relink the program
    m_glsl->linkProgram();

    // enable shaders
    m_glsl->bindShader();

    if (!m_grassTexture.load("data/grass.tga"))
    {
        std::cerr << "Could not load the grass texture!" << std::endl;
    }

    std::cout << "Succeeded in loading texture" << std::endl;

    //fixed function process
    glGenTextures(1, &m_grassTexID);
    glActiveTexture(GL_TEXTURE0); //?
    glBindTexture(GL_TEXTURE_2D, m_grassTexID);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D,0, GL_RGB8, m_grassTexture.getWidth(),
                      m_grassTexture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                      m_grassTexture.getImageData());

    /*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_grassTexture.getWidth(),
                      m_grassTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      m_grassTexture.getImageData());*/

    glEnableVertexAttribArray(0); // enable the vertex attribute
    glEnableVertexAttribArray(1); // enable the color attribute
    glEnableVertexAttribArray(2); // enable the color attribute

    //Return success
    return true;
}

void GLObject::prepare(float dt)
{
    if (m_angle > 360)
    {
        m_angle -= 360;
    } else {
        m_angle += 10.0 * dt;
    }
}

void GLObject::render()
{
    // modelview and projection matrices
    float modelviewMatrix[16];
    float projectionMatrix[16]; // 4 by 4

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Load the identity matrix (reset to the default position and orientation)
    glLoadIdentity();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glTranslatef(0.0f, -20.0f, 0.0f);
    glRotatef(25.0f, 1.0f, 0.0f, 0.0f);
    //Translate using our zPosition variable
    glTranslatef(0.0, 0.0, -50.0f);
    glRotatef(m_angle, 0.0f, 1.0f, 0.0f);

    // get the current matrices from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    m_glsl->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_glsl->sendUniform4x4("projection_matrix", projectionMatrix);
    m_glsl->sendUniform("texture0", 0);

    m_terrain.render();

}

void GLObject::shutdown()
{

}

void GLObject::onResize(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, float(width) / float(height), 1.0f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
