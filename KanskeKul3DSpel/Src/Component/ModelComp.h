#pragma once
#include <vector>
#include <unordered_map>
#include "System.h"
#include "glm/glm.hpp"
#include "Entity.h"
#include "TransformComp.h"
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
	ModelBuffer getBuffer(unsigned int meshID) const;
	unsigned int getMesh(Entity entity) const;
	void setColor(Entity entity, glm::vec3 color);
	glm::vec3 getColor(Entity entity) const;

	void setMesh(Entity entity, unsigned int mesh);

	void printImguiDebug(Entity entity);

private:
	struct Model
	{
		Entity owner;
		unsigned int mesh;
		glm::vec3 color;

		Model(Entity owner)
		{
			this->mesh = 0;
			this->owner = owner;
			this->color = glm::vec3(0, 0.9, 0.9);
		}
	};

	std::vector<Model> m_data;
	std::vector<ModelBuffer> m_meshes;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;

	ModelComp();

};