#include "SceneManager.h"
#include <iostream>
#include "../Entity/Entity.h"

SceneManager::SceneManager() :
	mCurrentScene(new Scene())
{
}

SceneManager::~SceneManager()
{
	std::cout << "Deleted Scene Manager\n";
}

void SceneManager::LoadNewScene(const std::string& levelName)
{
	if (mCurrentScene)
	{
		UnloadScene();

		mCurrentScene = new Scene();

		// Parse level file data
		std::cout << "Loading level: " << levelName << "\n";
	}
}

void SceneManager::UnloadScene()
{
	std::cout << "Unloading Scene\n";

	delete mCurrentScene;
	mCurrentScene = nullptr;
}

Entity* SceneManager::InstantiateEntity()
{
	Entity* e = nullptr;
	if (mCurrentScene)
	{
		e = mCurrentScene->CreateEntity();
	}
	return e;
}
