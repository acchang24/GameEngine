#pragma once
#include <unordered_map>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

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
	void ProcessInput(GLFWwindow* window);

	// Updates all the game logic such as player position/movement, enemies, AI, etc.
	// @param - A float representing delta time: the change in time between frames
	void Update(float deltaTime);

	// Sets all the buffers, swap chain, textures, vertex array objects, and renders to screen
	void Render();

	// Resizes and adjusts the viewport when the user changes the window size.
	// Registers on every window resize.
	// @param - GLFWwindow* for the window being resized
	// @param - int for the window's width
	// @param - int for the window's height
	static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);

private:
	// Unordered map of keyboard inputs on the previous frame/loop.
	// Takes key press as an int and stores the key's state as a bool.
	std::unordered_map<int, bool> mPrevInputs;

	// Pointer to a GLFWwindow, serves as the main game window.
	GLFWwindow* mWindow;

	// Bool to check if the game is running.
	bool mIsRunning;
};