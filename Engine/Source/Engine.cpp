#include "Engine.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"
#include "EngineUI/Editor.h"
#include "Util/Console.h"
#include "Util/LoggerMacros.h"

Engine::Engine(RendererMode renderMode, double mouseSensitivity, SDL_bool mouseCaptured) :
	mRenderer(renderMode),
	mKeyboard(),
	mMouse(mouseSensitivity, mouseCaptured),
	mPhysics(),
	mJobManager(JobManager::Get()),
	mAssetManager(AssetManager::Get()),
	mLogger(Logger::Get()),
	mEngineUI(this),
	mAudio(),
	mConsole(new Console(mLogger))
	//mEditor(new Editor())
{
	LOG_DEBUG("Started engine");

	mAssetManager->SetRenderer(&mRenderer);
}

Engine::~Engine()
{
}

bool Engine::Init(int windowWidth, int windowHeight, int subSamples, int v_sync, bool fullscreen, SDL_bool mouseCaptured, const char* gameName)
{
	mJobManager->Begin();

	if (!mRenderer.Init(windowWidth, windowHeight, subSamples, v_sync, fullscreen, mouseCaptured, gameName))
	{
		return false;
	}

	if (!mEngineUI.Init())
	{
		return false;
	}

	if (!mAudio.Init())
	{
		return false;
	}

	mMouse.CenterMouse(mRenderer.GetWindow());

	return true;
}

void Engine::Shutdown()
{
	LOG_DEBUG("Shutting down engine");

	mAudio.Shutdown();

	mEngineUI.Shutdown();

	mRenderer.Shutdown();

	mJobManager->End();

	mAssetManager->Shutdown();

	delete mConsole;

	//delete mEditor;
}
