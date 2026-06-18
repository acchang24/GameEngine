#include "Entity2D.h"
#include <iostream>

Entity2D::Entity2D() :
	Entity()
	//mScale(glm::vec2(1.0f, 1.0f))
{
}

Entity2D::~Entity2D()
{
	std::cout << "Deleted Entity2D\n";
}
