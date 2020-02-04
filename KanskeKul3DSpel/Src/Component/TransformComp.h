#pragma once
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#include "Entity.h"
#include "glm/gtc/matrix_transform.hpp"

class TransformComp
{
public:
	static TransformComp& get() 
	{
		static TransformComp instance;
		return instance;
	}

	void addTransform(Entity entity);
	void removeTransform(Entity entity);
	bool hasTransform(Entity entity) const { return m_dataMap.count(entity.id); }
	glm::mat4 getTransformMat(Entity entity)  const;
	glm::vec3 getPosition(Entity entity) const;
	void setPosition(Entity entity, glm::vec3 pos);
	glm::vec3 getScale(Entity entity) const;
	void setScale(Entity entity, glm::vec3 scale);
	glm::vec3 getRotation(Entity entity) const;
	void setRotation(Entity entity, glm::vec3 rotation);
	void move(Entity entity, glm::vec3 offset);

	glm::vec3 getRight(Entity entity) const;
	glm::vec3 getForward(Entity entity) const;
	glm::vec3 getUp(Entity entity) const;

	void printImguiDebug(Entity entity);

private:
	struct Transform
	{
		Entity owner;
		glm::vec3 pos;
		glm::vec3 scale;
		glm::vec3 rotation;
		glm::mat4 transform;
		bool dirty;

		Transform(Entity owner)
		{
			this->owner = owner;
			this->scale = {1, 1, 1};
			this->pos = {0, 0, 0};
			this->rotation = pos;
			this->transform = glm::identity<glm::mat4>();
			this->dirty = false;
		}
	};

	std::vector<Transform> m_data;
	std::unordered_map<unsigned int, unsigned int> m_dataMap;
};