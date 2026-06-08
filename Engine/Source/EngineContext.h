#pragma once

class Renderer;
class InputSystem;
class Physics;
class JobManager;
class AssetManager;
class EngineUI;
class AudioSystem;
class Logger;
class Editor;

struct EngineContext
{
	Renderer* renderer = nullptr;
	InputSystem* input = nullptr;
	Physics* physics = nullptr;
	JobManager* jobManager = nullptr;
	AssetManager* assetManager = nullptr;
	EngineUI* engineUI = nullptr;
	AudioSystem* audio = nullptr;
	Logger* logger = nullptr;
	Editor* editor = nullptr;
};
