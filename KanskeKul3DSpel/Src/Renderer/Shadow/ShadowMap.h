#pragma once
#include "GL/glew.h"

class ShadowMap
{
public:
	ShadowMap(unsigned int resolution, bool orthogonal = true);

	unsigned int getFrameBufferID() const { return m_frameBufferID; }
	unsigned int getRes() const { return m_res; }
	virtual ~ShadowMap(){}

private:
	unsigned int m_frameBufferID;
	unsigned int m_textureID;
	unsigned int m_res;
	bool m_orthogonal;
};
