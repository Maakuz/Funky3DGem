#pragma once
#include <vector>
#include <unordered_map>
#include "System.h"
#include "glm/glm.hpp"
#include "Entity.h"
constexpr unsigned int NO_MESH = 404;

class ModelComp : System
{
public:
	struct ModelBuffer 
	{
		ModelBuffer(unsigned int bufferID = NO_MESH, unsigned int size = 0)
		{
			this->size = size;
			this->vertexBufferID = bufferID;
			this->normalBufferID = bufferID;
		}

		unsigned int vertexBufferID;
		unsigned int normalBufferID;
		unsigned int size;
	};

	static ModelComp& get()
	{
		static ModelComp instance;
		return instance;
	}

	void addComponent(Entity entity);
	void removeComponent(Entity entity);
	bool hasComponent(Entity entity) const { return m_dataMap.count(entity.id); }
	ModelBuffer getBuffer(Entity entity) const;
	unsigned int getMesh(Entity entity) const;

	void setMesh(Entity entity, unsigned int mesh);

	void printImguiDebug(Entity entity);

private:
	struct Model
	{
		Entity owner;
		unsigned int mesh;

		Model(Entity owner)
		{
			this->mesh = 0;
			this->owner = owner;
		}
	};

	std::vector<Model> m_data;
	std::vector<ModelBuffer> m_meshes;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;

	ModelComp();

};