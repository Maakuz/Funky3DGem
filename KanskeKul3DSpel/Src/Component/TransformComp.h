#pragma once
#include <vector>
#include <unordered_map>
#include "System.h"
#include "glm/glm.hpp"
#include "Entity.h"
#include "glm/gtc/matrix_transform.hpp"

class TransformComp : System
{
public:
	static TransformComp& get() 
	{
		static TransformComp instance;
		return instance;
	}

	void addComponent(Entity entity);
	void removeComponent(Entity entity);
	bool hasComponent(Entity entity) const { return m_dataMap.count(entity.id); }

	glm::mat4 getTransformMat(Entity entity);
	glm::mat4 getInverseTransposeMat(Entity entity);
	glm::vec3 getPosition(Entity entity) const;
	void setPosition(Entity entity, glm::vec3 pos);
	glm::vec3 getScale(Entity entity) const;
	void setScale(Entity entity, glm::vec3 scale);
	glm::vec3 getRotation(Entity entity) const;
	void setRotation(Entity entity, glm::vec3 rotation);
	void move(Entity entity, glm::vec3 offset);

	glm::vec3 getRight(Entity entity);
	glm::vec3 getForward(Entity entity);
	glm::vec3 getUp(Entity entity);

	void printImguiDebug(Entity entity);

private:
	struct Transform
	{
		Entity owner;
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::mat4 transform;
		glm::mat4 invTranspose;
		bool dirty;

		Transform(Entity owner)
		{
			this->owner = owner;
			this->scale = {1, 1, 1};
			this->pos = {0, 0, 0};
			this->rotation = pos;
			this->transform = glm::identity<glm::mat4>();
			this->invTranspose = glm::identity<glm::mat4>();
			this->dirty = true;
		}
	};

	std::vector<Transform> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;

	void computeMat(Entity entity);
};