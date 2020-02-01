#pragma once
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Entity.h"
constexpr unsigned int NO_MESH = 404;

enum class Meshes
{
	Triangle,
	Cube
};

class ModelComp
{
public:
	struct MeshBuffer 
	{
		MeshBuffer(unsigned int bufferID = NO_MESH, unsigned int size = 0)
		{
			this->size = size;
			this->bufferID = bufferID;
		}

		unsigned int bufferID;
		unsigned int size;
	};

	static ModelComp& get()
	{
		static ModelComp instance;
		return instance;
	}

	void addModel(Entity entity);
	void removeModel(Entity entity);
	bool hasModel(Entity entity) { return m_dataMap.count(entity.id); }
	MeshBuffer getBuffer(Entity entity) const;
	Meshes getMesh(Entity entity) const;

	void setMesh(Entity entity, Meshes mesh);

private:
	struct Model
	{
		Entity owner;
		Meshes mesh;

		Model(Entity owner)
		{
			this->mesh = Meshes::Cube;
			this->owner = owner;
		}
	};

	std::vector<Model> m_data;
	std::unordered_map<Meshes, MeshBuffer> m_meshes;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;

	ModelComp();

};