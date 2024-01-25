#include "Skeleton.h"
#include <iostream>

Skeleton::Skeleton() :
	mNumBones(0)
{
}

Skeleton::~Skeleton()
{
	std::cout << "Delete skeleton" << std::endl;
}
