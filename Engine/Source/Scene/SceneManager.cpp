#include "SceneManager.h"
#include <iostream>
#include "../Entity/Entity.h"
#include "Scene.h"

SceneManager::SceneManager() :
	mCurrentScene(new Scene())
{
}

SceneManager::~SceneManager()
{
	std::cout << "Deleted Scene Manager\n";

	delete mCurrentScene;
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

void SceneManager::LoadNewScene()
{
	delete mCurrentScene;

	mCurrentScene = new Scene();
}
