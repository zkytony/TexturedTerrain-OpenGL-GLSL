#include "glslprogram.h"

GLSLProgram::GLSLProgram(const string &vertexShader, const string &fragmentShader)
{
    m_vertexShader.filename = vertexShader;
    m_fragmentShader.filename = fragmentShader;
}

bool GLSLProgram::initialize()
{
    m_programID = glCreateProgram(); // Create a GLSL program obj
    m_vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);

    // send source
    m_vertexShader.source = readFile(m_vertexShader.filename);
    m_fragmentShader.source = readFile(m_fragmentShader.filename);

    // check if the source string is empty
    if (m_vertexShader.source.empty() || m_fragmentShader.source.empty())
    {
        return false;
    }

    // compile shader source
    const GLchar *tmp = static_cast<const GLchar*>(m_vertexShader.source.c_str());
    glShaderSource(m_vertexShader.id, 1, (const GLchar**)&tmp, NULL);

    tmp = static_cast<const GLchar*>(m_fragmentShader.source.c_str());
    glShaderSource(m_fragmentShader.id, 1, (const GLchar**)&tmp, NULL);

    if (!compileShader(m_vertexShader) || !compileShader(m_fragmentShader))
    {
        std::cerr << "Could not compile the shaders, they are invalid";
        return false;
    }

    // attach shader to current GLSL program
    glAttachShader(m_programID, m_vertexShader.id);
    glAttachShader(m_programID, m_fragmentShader.id);

    // link the program;
    glLinkProgram(m_programID);
    return true;
}

void GLSLProgram::unload()
{
    glDetachShader(m_programID, m_vertexShader.id);
    glDetachShader(m_programID, m_fragmentShader.id);
    glDeleteShader(m_vertexShader.id);
    glDeleteShader(m_fragmentShader.id);
    glDeleteShader(m_programID);
}

void GLSLProgram::linkProgram()
{
    glLinkProgram(m_programID);
}

GLuint GLSLProgram::getUniformLocation(const string &name)
{
    map<string, GLuint>::iterator i = m_uniformMap.find(name);
    if (i == m_uniformMap.end())
    {
        GLuint location = glGetUniformLocation(m_programID, name.c_str());
        m_uniformMap.insert(std::make_pair(name, location));
        return location;
    }

    return (*i).second;
}

GLuint GLSLProgram::getAttribLocation(const string& name)
{
    map<string, GLuint>::iterator i = m_attribMap.find(name);
    if (i == m_attribMap.end())
    {
        GLuint location = glGetAttribLocation(m_programID, name.c_str());
        m_attribMap.insert(std::make_pair(name, location));
        return location;
    }

    return (*i).second;
}

void GLSLProgram::sendUniform4x4(const string& name, const float* matrix, bool transpose)
{
    GLuint location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, transpose, matrix);
}

void GLSLProgram::sendUniform3x3(const string& name, const float* matrix, bool transpose)
{
    GLuint location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, transpose, matrix);
}

void GLSLProgram::sendUniform(const string& name, const float red, const float green,
                    const float blue, const float alpha)
{
    GLuint location = getUniformLocation(name);
    glUniform4f(location, red, green, blue, alpha);
}

void GLSLProgram::sendUniform(const string& name, const float x, const float y, const float z)
{
    GLuint location = getUniformLocation(name);
    glUniform3f(location, x, y, z);
}

void GLSLProgram::sendUniform(const string& name, const float scalar)
{
    GLuint location = getUniformLocation(name);
    glUniform1f(location, scalar);
}

void GLSLProgram::bindAttrib(unsigned int index, const string& attribName)
{
    glBindAttribLocation(m_programID, index, attribName.c_str());
}

void GLSLProgram::bindShader()
{
    glUseProgram(m_programID);
}

string GLSLProgram::readFile(const string& filename)
{
    ifstream fileIn(filename.c_str());

    if (!fileIn.good())
    {
        std::cerr << "Could not load shader: " << filename << std::endl;
        return string();
    }

    string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));
    return stringBuffer;
}

bool GLSLProgram::compileShader(const GLSLShader& shader)
{
    glCompileShader(shader.id);
    GLint result = 0xDEADBEEF;
    glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        std::cout << "Could not compile shader: " << shader.id << std::endl;
        outputShaderLog(shader.id);
        return false;
    }

    return true;
}

void GLSLProgram::outputShaderLog(unsigned int shaderID)
{
    vector<char> infoLog;
    GLint infoLen;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
    infoLog.resize(infoLen);

    std::cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
    glGetShaderInfoLog(shaderID, sizeof(infoLog), &infoLen, &infoLog[0]);

    std::cerr << string(infoLog.begin(), infoLog.end()) << std::endl;
}
