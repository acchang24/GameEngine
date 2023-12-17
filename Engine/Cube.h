#pragma once
#include "Entity3D.h"

// 3D cube primitive
class Cube : public Entity3D
{
public:
	Cube();
	~Cube();

	// Cube specific update
	void OnUpdate(float deltaTime) override;
	// Cube specific draw
	void OnDraw() override;
private:
};
