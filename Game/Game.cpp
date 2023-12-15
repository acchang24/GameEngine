#include "Game.h"
#include <iostream>
#include "glm/glm.hpp"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexLayouts.h"
#include "Texture.h"

#define WIDTH 1280
#define HEIGHT 720

Game::Game() : 
	mWindow(nullptr),
	simpleShader(nullptr),
	vBuffer(nullptr),
	texture(nullptr),
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

	VertexColorTexture vertices[] = {
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), // Bottom left
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), // Bottom right
		glm::vec3(0.0f,  0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec2(0.5f, 1.0f) // Top
	};

	// Initialize a vertex buffer
	vBuffer = new VertexBuffer(vertices, 0, sizeof(vertices), 0, sizeof(vertices)/sizeof(VertexColorTexture), 0, VertexLayout::VertexColorTexture);
	// Compile shader
	simpleShader = new Shader("Shaders/colorTextureVS.glsl", "Shaders/colorTextureFS.glsl");

	// Create a new texture
	texture = new Texture("Assets/wall.jpg");

	return true;
}

void Game::Shutdown()
{
	glfwTerminate();
	delete simpleShader;
	delete vBuffer;
	delete texture;
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
	}
}

void Game::ProcessInput(GLFWwindow* window)
{
	// Check if user clicks on window close
	if (glfwWindowShouldClose(mWindow))
	{
		mIsRunning = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mIsRunning = false;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !mPrevInputs[GLFW_KEY_SPACE])
	{
		mPrevInputs[GLFW_KEY_SPACE] = true;
		std::cout << "space" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
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

	simpleShader->SetActive();
	texture->SetActive();
	//simpleShader->SetVec4("newColor", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
	vBuffer->Draw();

	glfwSwapBuffers(mWindow);
}

void Game::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	// Set the viewport to new width and height
	glViewport(0, 0, width, height);
}
