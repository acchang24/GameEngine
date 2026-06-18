#pragma once
#include "Entity.h"

class Entity2D : public Entity
{
public:
	Entity2D();
	virtual ~Entity2D();

	//// Gets the entity's float scale
	//// @return - float for the scale
	//const glm::vec2& GetScale2D() const { return mScale; }

	//// Sets the entity's float scale
	//// @param - float for x scale
	//// @param - float for y scale
	//void SetScale2D(float x, float y) { mScale.x = x; mScale.y = y; }

protected:
	// Entity's scale in float
	//glm::vec2 mScale;
};
