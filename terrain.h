#ifndef BOGLGP_TERRAIN_H
#define BOGLGP_TERRAIN_H

#ifdef _WIN32
#include <windows.h>
#endif

#include "headers.h"

using std::string;
using std::vector;

// convenient structures
struct Vertex
{
	float x, y, z;

	// it can have a constructor
	Vertex(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

struct Color
{
    float r, g, b;
    Color(float r, float g, float b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

// 2D
struct TexCoord
{
    float s, t;
    TexCoord(float s, float t)
    {
        this->s = s;
        this->t = t;
    }
};

class Terrain
{
public:
    Terrain(); // constructor
    void generateTexCoords(int width);
    bool loadHeightmap(const string& rawFile, int width);
    void render();

private:
    void generateVertices(const vector<float> heights, int width);
    void generateIndices(int width);

    unsigned int m_vertexBuffer; // This time VBOs are not GLuint
    unsigned int m_indexBuffer;
    unsigned int m_colorBuffer;
    unsigned int m_texCoordBuffer;

    vector<Vertex> m_vertices;
    vector<unsigned int> m_indices;
    vector<Color> m_colors;
    vector<TexCoord> m_texCoords;
};

#endif // BOGLGP_TERRAIN_h
