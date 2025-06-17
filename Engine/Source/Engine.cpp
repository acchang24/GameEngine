#include "Engine.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"

Engine::Engine(RendererMode renderMode, double mouseSensitivity) :
	mRenderer(renderMode),
	mKeyboard(),
	mLogger(),
	mMouse(mouseSensitivity),
	mPhysics(),
	mJobManager(JobManager::Get()),
	mAssetManager(AssetManager::Get()),
	mEditorUI(this),
	mAudio()
{
	mLogger.Log("Started engine", LogLevel::Info);
}

Engine::~Engine()
{
}

bool Engine::Init(int windowWidth, int windowHeight, int subSamples, int v_sync, bool fullscreen, SDL_bool mouseCaptured, const char* gameName)
{
	if (!mRenderer.Init(windowWidth, windowHeight, subSamples, v_sync, fullscreen, mouseCaptured, gameName))
	{
		return false;
	}

	mJobManager->Begin();

	if (!mEditorUI.Init())
	{
		return false;
	}

	if (!mAudio.Init())
	{
		return false;
	}

	return true;
}

void Engine::Shutdown()
{
	mLogger.Log("Shutting down engine", LogLevel::Info);

	mAudio.Shutdown();

	mEditorUI.Shutdown();

	mRenderer.Shutdown();

	mJobManager->End();

	mAssetManager->Shutdown();
}
