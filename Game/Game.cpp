#include "Game.h"
#include <iostream>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexLayouts.h"
#include "Texture.h"
#include "Entity3D.h"
#include "TimerComponent.h"
#include "Cube.h"
#include "Camera.h"

#define WIDTH 1280
#define HEIGHT 720

// view matrix
glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
// projection matrix
glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

Game::Game() :
	mWindow(nullptr),
	simpleShader(nullptr),
	texture(nullptr),
	mMousePosX(static_cast<double>(WIDTH / 2)),
	mMousePosY(static_cast<double>(HEIGHT / 2)),
	mCamera(nullptr),
	mousePrevX(static_cast<double>(WIDTH / 2)),
	mousePrevY(static_cast<double>(HEIGHT / 2)),
	mIsRunning(true),
	mFirstMouse(true)
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

	// Enable z-buffering
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// Compile shader
	simpleShader = new Shader("Shaders/textureVS.glsl", "Shaders/textureFS.glsl");

	// Create a new texture
	texture = new Texture("Assets/companioncube.png");

	// Create a camera
	mCamera = new Camera();
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	// Cube positions
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Create new cubes
	for (int i = 0; i < 10; ++i)
	{
		Cube* cube = new Cube();
		cube->SetPosition(cubePositions[i]);
		cube->SetShader(simpleShader);
		cube->SetTexture(texture);
		cube->SetYaw(20.0f * i);

		// Add a timer component to the cube
		TimerComponent* timer = new TimerComponent(cube);
		mEntities.emplace_back(cube);
	}

	// Initialize view proj and send to shader
	glm::mat4 viewProj = projection * view;
	simpleShader->SetActive();
	simpleShader->SetMat4("viewProjection", viewProj);

	return true;
}

void Game::Shutdown()
{
	glfwTerminate();

	for (auto e : mEntities)
	{
		delete e;
	}
	mEntities.clear();

	delete simpleShader;
	delete texture;
	delete mCamera;
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
	glfwGetCursorPos(mWindow, &mMousePosX, &mMousePosY);

	if (mFirstMouse)
	{
		mousePrevX = mMousePosX;
		mousePrevY = mMousePosY;
		mFirstMouse = false;
	}

	// Calculate mouse offset
	double xOffset = mMousePosX - mousePrevX;
	double yOffset = mousePrevY - mMousePosY; // reverse since y coordinates range bottom to top

	mousePrevX = mMousePosX;
	mousePrevY = mMousePosY;

	double sensitivity = 0.05;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	// Update camera angles
	mCamera->mYaw = mCamera->mYaw + xOffset;
	mCamera->mPitch = mCamera->mPitch + yOffset;
	
	if (mCamera->mPitch > 89.9)
	{
		mCamera->mPitch = 89.9;
	}
	if (mCamera->mPitch < -89.9)
	{
		mCamera->mPitch = -89.9;
	}

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
	for (auto e : mEntities)
	{
		e->Update(deltaTime);
	}
}

void Game::Render()
{
	// Specify color to clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// Clear the color buffer, depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	view = mCamera->SetActive();

	simpleShader->SetActive();
	simpleShader->SetMat4("viewProjection", projection * view);
	
	// DRAW
	for (auto e : mEntities)
	{
		e->Draw();
	}

	glfwSwapBuffers(mWindow);
}

void Game::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	// Set the viewport to new width and height
	glViewport(0, 0, width, height);

	// Set new width/height ratio for perspective projection matrix, and update the projection matrix
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
}
