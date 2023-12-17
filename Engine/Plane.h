#pragma once
#include "Entity3D.h"

// 3D plane primitive
class Plane : public Entity3D
{
public:
	Plane();
	~Plane();

	// Plane specific Update
	void OnUpdate(float deltaTime) override;
	// Plane specific Draw
	void OnDraw() override;

private:

};