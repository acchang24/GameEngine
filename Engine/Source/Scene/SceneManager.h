#pragma once

class Entity;
class Scene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	// Gets the current scene
	// @return - Scene* for the scene
	Scene* GetCurrentScene() { return mCurrentScene; }

	// Creates an entity and returns it
	// @return - Entity* for a new entity created in the scene
	Entity* InstantiateEntity();

	// Loads new scene
	void LoadNewScene();

private:
	// The current scene in the game
	Scene* mCurrentScene;
};
