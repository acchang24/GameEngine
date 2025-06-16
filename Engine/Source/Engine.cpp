#include "Engine.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"
#include "Util/Logger.h"


Engine::Engine(RendererMode renderMode, double mouseSensitivity) :
	mLogger(new Logger()),
	mRenderer(new Renderer(renderMode)),
	mJobManager(JobManager::Get()),
	mAssetManager(AssetManager::Get()),
	mKeyboard(),
	mMouse(mouseSensitivity),
	mPhysics(),
	mAudio()
{
	mLogger->Log("Started engine", LogLevel::Info);
}

Engine::~Engine()
{
	delete mLogger;

	delete mRenderer;
}

bool Engine::Init(int windowWidth, int windowHeight, int subSamples, int v_sync, bool fullscreen, SDL_bool mouseCaptured, const char* gameName)
{
	if (!mRenderer->Init(windowWidth, windowHeight, subSamples, v_sync, fullscreen, mouseCaptured, gameName))
	{
		return false;
	}

	mJobManager->Begin();

	if (!mAudio.Init())
	{
		return false;
	}

	return true;
}

void Engine::Shutdown()
{
	mLogger->Log("Shutting down engine", LogLevel::Info);

	mRenderer->Shutdown();

	mJobManager->End();

	mAssetManager->Shutdown();
}
