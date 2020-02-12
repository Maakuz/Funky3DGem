#pragma once
#include "GL/glew.h"

class shadowMap
{
public:
	shadowMap(unsigned int resolution = 1024, bool orthogonal = true);

	unsigned int getFrameBufferID() const { return m_frameBufferID; }
	unsigned int getRes() const { return m_res; }
	virtual ~shadowMap(){}

private:
	unsigned int m_frameBufferID;
	unsigned int m_textureID;
	unsigned int m_res;
	bool m_orthogonal;
};
