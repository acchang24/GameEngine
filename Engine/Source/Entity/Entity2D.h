#pragma once
#include "Entity.h"
#include <glm/glm.hpp>

class Entity2D : public Entity
{
public:
	Entity2D();

	// Entity2D constructor:
	// @param - float for width of entity
	// @param - float for height of entity
	Entity2D(float width, float height);
	virtual ~Entity2D();

	// Gets the entity's 2d position
	// @return - const glm::vec2& for the position
	const glm::vec2& GetPosition() const { return mPosition; }

	// Gets the entity's float scale
	// @return - float for the scale
	float GetScale() const { return mScale; }

	// Sets the entity's 2D position
	// @param - const glm::vec2& for new pos
	void SetPosition(const glm::vec2& pos) { mPosition = pos; }

	// Sets the entity's float scale
	// @param - float for the new scale
	void SetScale(float scale) { mScale = scale; }

protected:
	// Position for 2D
	glm::vec2 mPosition;

	// Entity's scale in float
	float mScale;
};
