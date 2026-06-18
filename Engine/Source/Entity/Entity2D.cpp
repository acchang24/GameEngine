#include "Entity2D.h"
#include <iostream>

Entity2D::Entity2D() :
	Entity(),
	mScale(1.0f)
{
}

Entity2D::~Entity2D()
{
	std::cout << "Deleted Entity2D\n";
}
