#include "Game.h"
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

Game::Game() : 
	mWindow(nullptr),
	mIsRunning(true)
{
	mPrevInputs[GLFW_KEY_ESCAPE] = false;
	mPrevInputs[GLFW_KEY_SPACE] = false;
}

Game::~Game()
{
	Shutdown();
}

bool Game::Init()
{
	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	// Use core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Game", NULL, NULL);
	if (!mWindow)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(mWindow);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Set viewport
	glViewport(0, 0, WIDTH, HEIGHT);

	// Register the callback function for when window gets resized
	glfwSetFramebufferSizeCallback(mWindow, FrameBufferSizeCallBack);

	// Enable v-sync by default
	glfwSwapInterval(1);

	return true;
}

void Game::Shutdown()
{
	glfwTerminate();
}

void Game::Run()
{
	float startTime = glfwGetTime();

	// Run the main game loop
	while (mIsRunning)
	{
		glfwPollEvents();

		// Calculate delta time
		float endTime = glfwGetTime();
		float deltaTime = endTime - startTime;
		// Set the new starting time stamp to the current end time stamp
		startTime = endTime;

		ProcessInput(mWindow);

		Update(deltaTime);

		Render();

		if (glfwWindowShouldClose(mWindow))
		{
			mIsRunning = false;
		}
	}
}

void Game::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mIsRunning = false;
		//glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !mPrevInputs[GLFW_KEY_SPACE])
	{
		mPrevInputs[GLFW_KEY_SPACE] = true;
		std::cout << "space" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && mPrevInputs[GLFW_KEY_SPACE])
	{
		mPrevInputs[GLFW_KEY_SPACE] = false;
	}
}

void Game::Update(float deltaTime)
{

}

void Game::Render()
{
	// Specify color to clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(mWindow);
}

void Game::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	// Set the viewport to new width and height
	glViewport(0, 0, width, height);
}
