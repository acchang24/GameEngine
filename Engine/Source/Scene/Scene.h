#pragma once
#include <vector>

class Entity;

// Scene is a collection of entities that make up a level/scene in a game world
class Scene
{
public:
	Scene();
	~Scene();

	// Creates a new empty entity and adds it to this scene
	// @return - Entity* for the newly created entity
	Entity* CreateEntity();

	// Gets the collection of entities in this scene
	// @return - const std::vector<Entity*>& for the vector of entities
	const std::vector<Entity*>& GetEntities() const { return mEntities; }

private:
	std::vector<Entity*> mEntities;
};
