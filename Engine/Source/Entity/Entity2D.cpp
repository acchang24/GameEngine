#include "Entity2D.h"
#include <iostream>

Entity2D::Entity2D() :
	Entity(),
	mPosition(glm::vec2(0.0f, 0.0f)),
	mScale(1.0f)
{
}

Entity2D::Entity2D(float width, float height) :
	Entity(),
	mPosition(glm::vec2(0.0f, 0.0f)),
	mScale(1.0f)
{
}

Entity2D::~Entity2D()
{
	std::cout << "Deleted Entity2D\n";
}
