#include <fstream>
#include <sstream>
#include <vector>
#include "Shader.h"
#include "Debug/ConsoleWindow.h"

Shader::Shader(std::string path, ShaderType type)
{
    m_id = create(path, type);
}

Shader::~Shader()
{
    glDeleteShader(m_id);
}

GLuint Shader::create(std::string path, ShaderType type)
{
    GLenum glType;

    switch (type)
    {
    case ShaderType::Vertex:
        glType = GL_VERTEX_SHADER;
        break;
    case ShaderType::Fragment:
        glType = GL_FRAGMENT_SHADER;
        break;
    default:
        glType = GL_VERTEX_SHADER;
        break;
    }

    GLuint id = glCreateShader(glType);


    std::string code;
    std::ifstream file(path, std::ios::in);
    if (file.is_open())
    {
        std::stringstream sstr;
        sstr << file.rdbuf();
        code = sstr.str();

        file.close();
    }

    else
        printf("Shader with path \"%s\" failed to open\n", path.c_str());


    //compile
    char const* codePtr = code.c_str();
    glShaderSource(id, 1, &codePtr, NULL);
    glCompileShader(id);

    GLint result = GL_FALSE;
    int logLength = 0;

    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(id, GL_COMPILE_STATUS, &logLength);

    if (logLength > 0)
    {
        std::vector<char> errorMsg(logLength + 1);
        glGetShaderInfoLog(id, logLength, NULL, &errorMsg[0]);
        printf("%s\n", &errorMsg[0]);
    }

    return id;
}

Program::Program(std::initializer_list<Shader> shaders)
{
    m_id = glCreateProgram();

    for (Shader shader : shaders)
        glAttachShader(m_id, shader.getID());

    glLinkProgram(m_id);

    GLint result = GL_FALSE;
    int logLength = 0;

    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> errorMsg(logLength + 1);
        glGetProgramInfoLog(m_id, logLength, NULL, &errorMsg[0]);
        printf("%s\n", &errorMsg[0]);
    }

    for (Shader shader : shaders)
        glDetachShader(m_id, shader.getID());
}

Program::~Program()
{
    glDeleteProgram(m_id);
}

void Program::use()
{
    glUseProgram(m_id);
}

GLuint Program::initializeUniformLocation(std::string name)
{
    GLuint id = 0;
    if (!m_uniformLocations.count(name))
    {
         id = glGetUniformLocation(m_id, name.c_str());
         m_uniformLocations.emplace(name, id);
    }



    return id;
}

GLuint Program::getUniformID(std::string name) const
{
    GLuint id = 0;

    if (m_uniformLocations.count(name))
        id = m_uniformLocations.at(name);

    else
        printfCon("Uniform %s does not exist", name.c_str());

    return id;
}
