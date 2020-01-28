#pragma once
#include <string>
#include <GL/glew.h>

#define SHADER_PATH "../Resources/Shaders/"

enum class ShaderType
{
    Vertex,
    Fragment
};

class Shader
{
public:
    Shader(std::string path, ShaderType type);
    virtual ~Shader();

    GLuint getID() const { return m_ID; }

private:
    GLuint m_ID;

    static GLuint create(std::string path, ShaderType type);
};

class Program
{
public:
	Program(std::initializer_list<Shader> shaders);
	virtual ~Program();

    void use();

private:
    GLuint m_ID;
};