#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "DefferedPass.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer() {};

	void draw();
	void rendererDebug();

	static void queueModel(Entity entity);

private:
	DefferedPass m_deffered;
	Program m_forward;
	Program m_shadowBuffer;
	
	static std::vector<Entity> s_modelQueue;

	unsigned int m_quadID;

	bool m_showDebug;
};