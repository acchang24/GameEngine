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

	delete mCurrentScene;
}

void SceneManager::LoadNewScene(const std::string& levelName)
{
	if (mCurrentScene)
	{
		// Clear old level and create a new one
		delete mCurrentScene;

		mCurrentScene = new Scene();

		// Parse level file data
		std::cout << "Loading level: " << levelName << "\n";
	}
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

