#include "Scene.h"
#include <algorithm>
#include <iostream>
#include "../Entity/Entity.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	std::cout << "Deleted scene\n";

	for (Entity* e : mEntities)
	{
		delete e;
	}
	mEntities.clear();
}

Entity* Scene::CreateEntity()
{
	Entity* e = new Entity();

	mEntities.emplace_back(e);

	return e;
}

void Scene::ClearDestoyedEntities()
{
	for (Entity* entity : mEntitiesToDelete)
	{
		DeleteGameEntity(entity);
	}
	mEntitiesToDelete.clear();
}

void Scene::DeleteGameEntity(Entity* e)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), e);
	if (iter != mEntities.end())
	{
		// Swap to end of vector and pop off
		auto iter2 = mEntities.end() - 1;
		std::iter_swap(iter, iter2);
		delete e;
		mEntities.pop_back();
	}
}
