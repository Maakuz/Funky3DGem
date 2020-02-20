#pragma once
#include <string>
#include <GL/glew.h>
#include <unordered_map>

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

    GLuint getID() const { return m_id; }

private:
    GLuint m_id;

    static GLuint create(std::string path, ShaderType type);
};

class Program
{
public:
	Program(std::initializer_list<Shader> shaders);
	virtual ~Program();

    void use();
    GLuint getID() const { return m_id; }

    GLuint initializeUniformLocation(std::string name);
    GLuint getUniformID(std::string name) const;

private:
    GLuint m_id;
    std::unordered_map<std::string, GLuint> m_uniformLocations;
};