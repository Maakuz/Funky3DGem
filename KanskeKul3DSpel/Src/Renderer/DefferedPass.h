#pragma once
#include "GL/glew.h"
#include "Component/Entity.h"
#include "Renderer/Shader.h"
#include <vector>

class DefferedPass
{
public:
	DefferedPass();

	~DefferedPass(){}

	void draw(std::vector<Entity>* modelQueue);

	unsigned int getPos() const { return m_posBuffer; }
	unsigned int getNormal() const { return m_normalBuffer; }
	unsigned int getColor() const { return m_colorBuffer; }
	unsigned int getGBufferID() const { return m_gBuffer; }

private:
	unsigned int m_posBuffer;
	unsigned int m_normalBuffer;
	unsigned int m_colorBuffer; //might bake in spec here in the last bit
	unsigned int m_gBuffer;

	Program m_deffered;
};
