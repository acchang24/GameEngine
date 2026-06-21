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

	// Creates an entity and returns it
	// @return - Entity* for a new entity created in the scene
	Entity* InstantiateEntity();

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
