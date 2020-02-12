#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "Component/Entity.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer() {};

	void draw();
	void rendererDebug();

	static void queueModel(Entity entity);

private:
	Program m_forward;
	Program m_shadowBuffer;
	
	static std::vector<Entity> s_modelQueue;

	bool m_showDebug;
};