#pragma once
#include <string>
#include "Scene.h"

class Entity;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	// Loads new scene
	// @param - const std::string& for the level name
	void LoadNewScene(const std::string& levelName);

	// Unloads scene and deletes all entities
	void UnloadScene();

	// Creates an entity and returns it
	// @return - Entity* for a new entity created in the scene
	Entity* InstantiateEntity();

	// Adds entity to the entity vector in scene
	// @param - Entity* for the entity
	void AddEntity(Entity* e) { mCurrentScene->AddEntity(e); }

	// Adds an entity marked EntityState::Destroy to the scene's vector of entities to destroy
	void RemoveEntity(Entity* e) { mCurrentScene->AddEntityToDestroy(e); }

	// Deletes all entities that are marked EntityState::Destroy in the scene
	void ClearDestoyedEntities() { mCurrentScene->ClearDestoyedEntities(); }

	// Gets the current scene
	// @return - Scene* for the scene
	Scene* GetCurrentScene() { return mCurrentScene; }

private:
	// The current scene in the game
	Scene* mCurrentScene;
};
