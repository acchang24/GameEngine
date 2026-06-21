#include "Engine.h"

Engine::Engine(RendererMode renderMode) :
	mLogger(),
	mRenderer(renderMode),
	mInputSystem(),
	mPhysics(),
	mJobManager(),
	mAssetManager(),
	mSceneManager(),
	mEngineUI(this),
	mAudio(),
	mEditor()
{
	LOG_DEBUG("Started engine");
}

Engine::~Engine()
{
	std::cout << "Deleted engine\n";
}

bool Engine::Init(int windowWidth, int windowHeight, int subSamples, int v_sync, bool fullscreen, double mouseSensitivity, SDL_bool mouseCaptured, const char* gameName)
{
	mJobManager.Begin();

	if (!mRenderer.Init(windowWidth, windowHeight, subSamples, v_sync, fullscreen, mouseCaptured, gameName))
	{
		return false;
	}

	if (!mInputSystem.Init(mRenderer.GetWindow(), mouseSensitivity, mouseCaptured))
	{
		return false;
	}

	if (!mEngineUI.Init(&mRenderer))
	{
		return false;
	}

	if (!mAudio.Init())
	{
		return false;
	}

	// Populate context
	mContext.renderer = &mRenderer;
	mContext.input = &mInputSystem;
	mContext.physics = &mPhysics;
	mContext.jobManager = &mJobManager;
	mContext.assetManager = &mAssetManager;
	mContext.sceneManager = &mSceneManager;
	mContext.engineUI = &mEngineUI;
	mContext.audio = &mAudio;
	mContext.logger = &mLogger;
	mContext.editor = &mEditor;

	// Bridge logger macro system to this engine's logger instance
	Log::ActiveLogger = &mLogger;

	AssetBridge::ActiveManager = &mAssetManager;

	// Center the mouse
	mInputSystem.CenterMouse();

	return true;
}

void Engine::Shutdown()
{
	LOG_DEBUG("Shutting down engine");

	mSceneManager.UnloadScene();

	mAudio.Shutdown();

	mEngineUI.Shutdown();

	mRenderer.Shutdown();

	mJobManager.End();

	mAssetManager.Shutdown();
}
