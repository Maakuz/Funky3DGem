#include "Shader.h"
#include <fstream>
#include <sstream>
#include <vector>

Shader::Shader(std::string path, ShaderType type)
{
    m_ID = create(path, type);
}

Shader::~Shader()
{
    glDeleteShader(m_ID);
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
    m_ID = glCreateProgram();

    for (Shader shader : shaders)
        glAttachShader(m_ID, shader.getID());

    glLinkProgram(m_ID);

    GLint result = GL_FALSE;
    int logLength = 0;

    glGetProgramiv(m_ID, GL_LINK_STATUS, &result);
    glGetProgramiv(m_ID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> errorMsg(logLength + 1);
        glGetProgramInfoLog(m_ID, logLength, NULL, &errorMsg[0]);
        printf("%s\n", &errorMsg[0]);
    }

    for (Shader shader : shaders)
        glDetachShader(m_ID, shader.getID());
}

Program::~Program()
{
    glDeleteProgram(m_ID);
}

void Program::use()
{
    glUseProgram(m_ID);
}
