#pragma once
#include "Entity.h"

class Entity2D : public Entity
{
public:
	Entity2D();
	virtual ~Entity2D();

	// Gets the entity's float scale
	// @return - float for the scale
	float GetScale() const { return mScale; }

	// Sets the entity's float scale
	// @param - float for the new scale
	void SetScale(float scale) { mScale = scale; }

protected:
	// Entity's scale in float
	float mScale;
};
