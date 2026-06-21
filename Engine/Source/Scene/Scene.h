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

	// Deletes all entities that are marked EntityState::Destroy
	void ClearDestoyedEntities();

	// Adds entity to the entity vector
	// @param - Entity* for the entity
	void AddEntity(Entity* e) { mEntities.emplace_back(e); }

	// Adds an entity to the destroy vector: mEntitiesToDelete
	void AddEntityToDestroy(Entity* e) { mEntitiesToDelete.emplace_back(e); }

	// Deletes an entity from the vector of entities, and deletes that entity from memory
	// @param - Entity* for the entity to remove
	void DeleteGameEntity(Entity* e);

	// Gets the collection of entities in this scene
	// @return - const std::vector<Entity*>& for the vector of entities
	const std::vector<Entity*>& GetEntities() const { return mEntities; }

private:
	// Vector of entities in game
	std::vector<Entity*> mEntities;

	// Vector of entities to delete or remove
	std::vector<Entity*> mEntitiesToDelete;
};
