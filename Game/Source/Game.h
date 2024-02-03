#pragma once
#include <unordered_map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader;
class Texture;
class Entity;
class Camera;
class AssetManager;
class FrameBuffer;
class Skybox;
class Lights;
class ShadowMap;
class JobManager;

class Game
{
public:
	Game();
	~Game();

	// Initializes the game, GLFW library, and any other things required for the game
	// @return - Returns true if successfully initialized, false if not.
	bool Init();

	// De-allocates any resources and end the game.
	void Shutdown();

	// Runs the main game loop.
	void Run();

	// Processes any keyboard, mouse, or controller inputs.
	// Typically called near the beginning of the render loop.
	// @param - pointer to a GLFWwindow
	// @param - float for delta time
	void ProcessInput(GLFWwindow* window, float deltaTime);

	// Updates all the game logic such as player position/movement, enemies, AI, etc.
	// @param - A float representing delta time: the change in time between frames
	void Update(float deltaTime);

	// Sets all the buffers, swap chain, textures, vertex array objects, and renders to screen
	void Render();

	void RenderScene();

	void RenderScene(Shader* shader);

	void ProcessMouseInput(GLFWwindow* window);

	// Resizes and adjusts the viewport as well as updates the perspective matrix
	// when the user changes the window size. Registers on every window resize.
	// @param - GLFWwindow* for the window being resized
	// @param - int for the window's width
	// @param - int for the window's height
	static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);

	// Adds an entity to the game's vector of entities
	// @param - Entity* for the new entity
	void AddGameEntity(Entity* e) { mEntities.emplace_back(e); }

private:
	// std::unordered_map of keyboard inputs on the previous frame/loop.
	// Takes key press as an int and stores the key's state as a bool.
	std::unordered_map<int, bool> mPrevInputs;

	// std::vector of game entities
	std::vector<Entity*> mEntities;

	// Pointer to a GLFWwindow, serves as the main game window.
	GLFWwindow* mWindow;

	// Pointer to a (static) asset manager
	AssetManager* mAssetManager;

	// The game's camera
	Camera* mCamera;

	// Frame buffer
	FrameBuffer* mFrameBuffer;

	// Skybox
	Skybox* mSkybox;

	// Use lighting
	Lights* mLights;

	// ShadowMap for shadows
	ShadowMap* mShadowMap;

	// Pointer to a static JobManager
	JobManager* mJobManager;

	double mMousePosX;
	double mMousePosY;

	double mMousePrevX;
	double mMousePrevY;

	bool mFirstMouse;

	// Bool to check if the game is running.
	bool mIsRunning;

	bool hdr;
};
