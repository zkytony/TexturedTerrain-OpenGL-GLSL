#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

#include "headers.h"

using std::string;
using std::map;
using std::ifstream;
using std::vector;

class GLSLProgram
{
    public:
        struct GLSLShader
        {
            unsigned int id;
            string filename;
            string source;
        };

        GLSLProgram(const string &vertexShader, const string &ragmentShader);
        bool initialize();
        void unload();
        void linkProgram();
        GLuint getUniformLocation(const string &name);
        GLuint getAttribLocation(const string &name);
        void sendUniform4x4(const string& name, const float* matrix, bool transpose=false);
        void sendUniform3x3(const string& name, const float* matrix, bool transpose=false);
        void sendUniform(const string& name, const float red, const float green,
                    const float blue, const float alpha);
        void sendUniform(const string& name, const float x, const float y, const float z);
        void sendUniform(const string& name, const float scalar);

        void bindAttrib(unsigned int index, const string &attribName);
        void bindShader(); // use program

    private:
        string readFile(const string &filename);
        bool compileShader(const GLSLShader &shader);
        void outputShaderLog(unsigned int shaderID);

        GLSLShader m_vertexShader, m_fragmentShader;
        unsigned int m_programID; // GLSL program ID
        map<string, GLuint> m_uniformMap; // for caching
        map<string, GLuint> m_attribMap;
};

#endif // GLSLPROGRAM_H
