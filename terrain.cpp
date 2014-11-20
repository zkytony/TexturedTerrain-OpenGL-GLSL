//This is an important program. A start/basis for one type
//of OpenGL programming

#include "terrain.h"
#include "globject.h"

Terrain::Terrain()
{
    // initialize VBOs
    m_vertexBuffer = m_indexBuffer = m_colorBuffer = 0;
}

void Terrain::generateVertices(const vector<float> heights, int width)
{
    // generate the vertices based on the given height and width
    int i = 0;

    // iterate from the far most to near most
    for (float z = float(-width / 2); z <= (width / 2); z++)
    {
        // iterate from the left most to right most
        for (float x = float(-width / 2); x <= (width / 2); x++)
        {
            // i++ post increment. Increment i after this line
            m_vertices.push_back(Vertex(x, heights[i++], z));
        }
    }

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size() * 3,
                 &m_vertices[0], GL_STATIC_DRAW);
}

void Terrain::generateIndices(int width)
{
    /*
        BASIC IDEA:
        We loop through building the triangles that
        make up each grid square in the heightmap

        (z*w+x) *----* (z*w+x+1)
                |   /|
                |  / |
                | /  |
     ((z+1)*w+x)*----* ((z+1)*w+x+1)

        using triangles to render grid by grid
    */

    // because it is just generating index, it does not matter
    // about the real value, only the order matters. Therefore,
    // loop from zero.
    for (int z = 0; z < width - 1; z++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            m_indices.push_back((z * width) + x); //Current point
            m_indices.push_back(((z + 1) * width) + x); //Next row
            m_indices.push_back((z * width) + x + 1); //Same row, but next column

            m_indices.push_back(((z + 1) * width) + x); //Next row
            m_indices.push_back(((z + 1) * width) + x + 1); //Next row, next column
            m_indices.push_back((z * width) + x + 1); //Same row, but next column
        }
    }

    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
}

void Terrain::generateTexCoords(int width)
{
    for (int z = 0; z < width; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            float s = (float(x) / float(width)) * 8.0f; // 8 bytes
            float t = (float(z) / float(width)) * 8.0f;
            m_texCoords.push_back(TexCoord(s, t));
        }
    }

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_texCoords.size()*2, &m_texCoords[0], GL_STATIC_DRAW);
}

// same as initializing the vertices, indices and the color buffer
bool Terrain::loadHeightmap(const string& rawFile, int width)
{
    const float HEIGHT_SCALE = 10.0f;
    // learn this
    std::ifstream fileIn(rawFile.c_str(), std::ios::binary);

    if (!fileIn.good())
    {
        std::cout << "File does not exist" << std::endl;
        return false;
    }

    //This line reads in the whole file into a string
    string stringBuffer(std::istreambuf_iterator<char>(fileIn),
                        (std::istreambuf_iterator<char>()));

    fileIn.close();

    if (stringBuffer.size() != (width * width))
    {
        std::cout << "Image size does not match passed width" << std:: endl;
        return false;
    }

    vector<float> heights;
    heights.reserve(width * width); // Reserve some space (faster)

    for (int i = 0; i < (width * width); i++)
    {
        /*
            if you look at the raw file, it is like some arrays.
            So stringBuffer basically converts the wholefile into
            a string array of characters. Then you can take each element
            out by treating the string as an array,

            because of this constructing code:
            string stringBuffer(std::istreambuf_iterator<char>(fileIn),
            (std::istreambuf_iterator<char>()));
        */

        //Convert to floating value, the unsigned char cast is importantant otherwise the values wrap at 128
        float value = (float)(unsigned char)stringBuffer[i] / 256.0f;

        heights.push_back(value * HEIGHT_SCALE);
        m_colors.push_back(Color(value, value, value));
    }

    glGenBuffers(1, &m_colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_colors.size() * 3, &m_colors[0], GL_STATIC_DRAW);

    generateVertices(heights, width);
    generateIndices(width);
    generateTexCoords(width);// generate texture coordinates

    return true;
}

void Terrain::render()
{
    // Bind Buffer and Set Vertex pointers
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    // forget about glVertexPointer(3, GL_FLOAT, 0, 0);
    // it's time to use VertexAttrib because it's glsl era
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glVertexAttribPointer((GLint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}
