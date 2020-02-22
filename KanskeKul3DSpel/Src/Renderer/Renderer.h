#pragma once
#include <GL/glew.h>
#include "Shader.h"
#include "DeferredPass.h"
#include "SkyDome.h"

class Renderer
{
public:
	Renderer();
	virtual ~Renderer() {};

	void draw();
	void rendererDebug();

	static void queueModel(Entity entity);

private:
	DeferredPass m_deffered;
	Program m_shadowBuffer;
	SkyDome m_sky;
	
	static std::vector<Entity> s_modelQueue;

	bool m_showDebug;
};