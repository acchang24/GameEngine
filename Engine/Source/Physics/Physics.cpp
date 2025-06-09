#include "Physics.h"
#include <algorithm>
#include <iostream>

Physics::Physics()
{

}

Physics::~Physics()
{
	std::cout << "Deleted Physics\n";
}

void Physics::RemoveCollider(CollisionComponent* collider)
{
	auto iter = std::find(mColliders.begin(), mColliders.end(), collider);
	if (iter != mColliders.end())
	{
		mColliders.erase(iter);
	}
}
