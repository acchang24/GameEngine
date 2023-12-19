#pragma once
#include "Entity3D.h"
#include <glm/glm.hpp>

// 3D sphere primitive
class Sphere :public Entity3D
{
public:
	Sphere(const glm::vec4& color);
	~Sphere();

	// Sphere specific update
	void OnUpdate(float deltaTime) override;
	// Sphere specific draw
	void OnDraw() override;
private:

};