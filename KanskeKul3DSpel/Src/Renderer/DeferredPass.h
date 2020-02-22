#pragma once
#include "GL/glew.h"
#include "Component/Entity.h"
#include "Renderer/Shader.h"
#include <vector>
#include <glm/vec3.hpp>

class DeferredPass
{
public:
	DeferredPass();

	~DeferredPass(){}

	void firstPass(std::vector<Entity>* modelQueue);
	void secondPass(const glm::vec3* firstDirLight, int lightCount);

	unsigned int getPos() const { return m_posBuffer; }
	unsigned int getNormal() const { return m_normalBuffer; }
	unsigned int getColor() const { return m_colorBuffer; }
	unsigned int getFBO() const { return m_gBuffer; }
	unsigned int getDepth() const { return m_depthBuffer; }

private:
	unsigned int m_posBuffer;
	unsigned int m_normalBuffer;
	unsigned int m_colorBuffer; //might bake in spec here in the last bit
	unsigned int m_depthBuffer;
	unsigned int m_gBuffer;

	Program m_first;
	Program m_second;
};
