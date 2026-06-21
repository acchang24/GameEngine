#include "Scene.h"
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
