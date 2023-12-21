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
#include "Sphere.h"
#include "Camera.h"
#include "AssetManager.h"
#include "Cache.h"
#include "Material.h"

#define WIDTH 1280
#define HEIGHT 720

// projection matrix
glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

Game::Game() :
	mWindow(nullptr),
	mCamera(nullptr),
	mMousePosX(static_cast<double>(WIDTH / 2)),
	mMousePosY(static_cast<double>(HEIGHT / 2)),
	mMousePrevX(static_cast<double>(WIDTH / 2)),
	mMousePrevY(static_cast<double>(HEIGHT / 2)),
	mFirstMouse(true),
	mIsRunning(true)
{
	mPrevInputs[GLFW_KEY_ESCAPE] = false;
	mPrevInputs[GLFW_KEY_SPACE] = false;
}

Game::~Game()
{
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

	glm::vec3 lightPosition(1.0f, 0.0f, 1.0f);

	AssetManager* am = AssetManager::Get();

	Shader* textureShader = new Shader("Shaders/textureVS.glsl", "Shaders/textureFS.glsl");
	Shader* colorShader = new Shader("Shaders/colorVS.glsl", "Shaders/colorFS.glsl");
	Shader* lightShader = new Shader("Shaders/simpleLightVS.glsl", "Shaders/simpleLightFS.glsl");
	lightShader->SetActive();
	lightShader->SetVec4("lightColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightShader->SetVec3("lightPos", lightPosition);

	Texture* texture = new Texture("Assets/companioncube.png");
	Texture* texture2 = new Texture("Assets/wall.jpg");

	am->SaveShader("texture", textureShader);
	am->SaveShader("color", colorShader);
	am->SaveShader("lightShader", lightShader);
	am->SaveTexture("Assets/companioncube.png", texture);
	am->SaveTexture("Assets/wall.jpg", texture2);

	mCamera = new Camera();
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	MaterialColors cubeMat = {glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f), 0.5f, 32.0f, true, false};
	Material* cubeMaterial = new Material();
	cubeMaterial->SetMaterialColors(cubeMat);
	cubeMaterial->SetShader(lightShader);
	cubeMaterial->AddTexture(texture);

	MaterialColors lightMat = { glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f), 0.0f, 0.0f, false, false };
	Material* lightMaterial = new Material();
	lightMaterial->SetMaterialColors(lightMat);
	lightMaterial->SetShader(colorShader);

	glm::vec3 objectPositions[] = {
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

	// Create new objects
	for (int i = 0; i < 10; ++i)
	{
		Cube* object = new Cube();
		object->SetPosition(objectPositions[i]);
		object->SetScale(0.5f);
		Material* mat = new Material(*cubeMaterial);
		if (i == 3 || i == 7)
		{
			mat->ChangeTexture(0, texture2);
		}
		object->SetMaterial(mat);
		object->SetYaw(25.0f);
		TimerComponent* timer = new TimerComponent(object);
		mEntities.emplace_back(object);
	}

	Sphere* lightSphere = new Sphere(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSphere->SetPosition(lightPosition);
	lightSphere->SetScale(0.1f);
	lightSphere->SetMaterial(new Material(*lightMaterial));
	mEntities.emplace_back(lightSphere);

	delete cubeMaterial;
	delete lightMaterial;

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

	delete mCamera;
}

void Game::Run()
{
	float startTime = glfwGetTime();

	while (mIsRunning)
	{
		glfwPollEvents();

		// Calculate delta time
		float endTime = glfwGetTime();
		float deltaTime = endTime - startTime;
		// Set the new starting time stamp to the current end time stamp
		startTime = endTime;

		ProcessInput(mWindow, deltaTime);

		Update(deltaTime);

		Render();
	}
}

void Game::ProcessInput(GLFWwindow* window, float deltaTime)
{
	ProcessMouseInput(window);

	// Check if user clicks on window close
	if (glfwWindowShouldClose(mWindow))
	{
		mIsRunning = false;
	}

	// Camera
	CameraMode mode = mCamera->GetCameraMode();
	glm::vec3 right = mCamera->GetRight();
	glm::vec3 up = mCamera->GetUp();
	float speed = 5.0f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		switch (mode)
		{
		case CameraMode::First:
			// Cross product between up and right vector to get the forward/backward dir
			mCamera->SetPosition(mCamera->GetPosition() + glm::normalize(glm::cross(up, right)) * speed * deltaTime);
			break;
		case CameraMode::Fly:
			// Use the camera's forward
			mCamera->SetPosition(mCamera->GetPosition() + mCamera->GetForward() * speed * deltaTime);
			break;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		switch (mode)
		{
		case CameraMode::First:
			// Cross product between up and right vector to get the forward/backward dir
			mCamera->SetPosition(mCamera->GetPosition() - glm::normalize(glm::cross(up, right)) * speed * deltaTime);
			break;
		case CameraMode::Fly:
			// Use the camera's forward
			mCamera->SetPosition(mCamera->GetPosition() - mCamera->GetForward() * speed * deltaTime);
			break;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		if (mode == CameraMode::First || mode == CameraMode::Fly)
		{
			mCamera->SetPosition(mCamera->GetPosition() - right * speed * deltaTime);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		if (mode == CameraMode::First || mode == CameraMode::Fly)
		{
			mCamera->SetPosition(mCamera->GetPosition() + right * speed * deltaTime);
		}
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

	mCamera->SetActive();

	std::unordered_map<std::string, Shader*>& shaders = AssetManager::Get()->GetShaderCache()->GetAssetMap();
	for (auto& s : shaders)
	{
		s.second->SetActive();
		s.second->SetMat4("viewProjection", projection * mCamera->GetViewMatrix());
		s.second->SetVec3("viewPos", mCamera->GetPosition());
	}

	for (auto e : mEntities)
	{
		e->Draw();
	}

	glfwSwapBuffers(mWindow);
}

void Game::ProcessMouseInput(GLFWwindow* window)
{
	glfwGetCursorPos(mWindow, &mMousePosX, &mMousePosY);

	if (mFirstMouse)
	{
		mMousePrevX = mMousePosX;
		mMousePrevY = mMousePosY;
		mFirstMouse = false;
	}

	// Calculate mouse offset
	double xOffset = mMousePosX - mMousePrevX;
	double yOffset = mMousePrevY - mMousePosY; // reverse since y coordinates range bottom to top

	mMousePrevX = mMousePosX;
	mMousePrevY = mMousePosY;

	double sensitivity = 0.05;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	// Update camera angles
	mCamera->mYaw = mCamera->mYaw + xOffset;
	mCamera->mPitch = mCamera->mPitch + yOffset;

	if (mCamera->mPitch >= 89.0)
	{
		mCamera->mPitch = 89.0;
		yOffset = 0.0f;
	}
	if (mCamera->mPitch <= -89.0)
	{
		mCamera->mPitch = -89.0;
		yOffset = 0.0f;
	}

	if (mCamera->GetCameraMode() == CameraMode::Orbit)
	{
		glm::vec3 camPos = mCamera->GetPosition();
		glm::vec3 camUp = mCamera->GetUp();
		glm::vec3 camTarget = mCamera->GetTarget();

		// Temp vec4 for camera position
		glm::vec4 pos(camPos, 1.0f);
		// Temp vec4 for the camera's pivot point
		glm::vec4 pivot(camTarget, 1.0f);

		// Calculate rotation matrix along y axis (yaw)
		glm::mat4x4 rotationX(1.0f);
		// Rotate based on xOffset
		rotationX = glm::rotate(rotationX, glm::radians(static_cast<float>(-xOffset)), camUp);
		pos = (rotationX * (pos - pivot)) + pivot;

		// Calculate rotation matrix along x axis (pitch)
		glm::mat4x4 rotationY(1.0f);
		// Rotate based on yOffset
		glm::vec3 right = glm::normalize(glm::cross(camUp, glm::normalize(camTarget - glm::vec3(pos.x, pos.y, pos.z))));
		rotationY = glm::rotate(rotationY, glm::radians(static_cast<float>(-yOffset)), right);
		pos = (rotationY * (pos - pivot)) + pivot;

		mCamera->SetPosition(glm::vec3(pos.x, pos.y, pos.z));
	}
}

void Game::FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	// Set the viewport to new width and height
	glViewport(0, 0, width, height);

	// Set new width/height ratio for perspective projection matrix, and update the projection matrix
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);
}
