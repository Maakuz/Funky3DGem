#pragma once
#include "GL/glew.h"
#include "Component/ModelComp.h"
#include "Shader.h"

class SkyDome
{
public:
	SkyDome();
	virtual ~SkyDome() {};

	void renderSky(unsigned int geometryFBO);

private:
	ModelComp::ModelBuffer m_skyModel;

	Program m_shader;
};