#include "Game.h"
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
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
#include "Material.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "Plane.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "Mesh.h"

int windowWidth = 1280;
int windowHeight = 720;

float nearPlane = 0.1f;
float farPlane = 10000.0f;

// projection matrix
glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), nearPlane, farPlane);

Game::Game() :
	mWindow(nullptr),
	mCamera(nullptr),
	mFrameBuffer(nullptr),
	mSkybox(nullptr),
	mLightArrays({}),
	uboCamera(0),
	mMousePosX(static_cast<double>(windowWidth / 2)),
	mMousePosY(static_cast<double>(windowHeight / 2)),
	mMousePrevX(static_cast<double>(windowHeight / 2)),
	mMousePrevY(static_cast<double>(windowHeight / 2)),
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

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//mWindow = glfwCreateWindow(mode->width, mode->height, "Game", monitor, NULL);

	mWindow = glfwCreateWindow(windowWidth, windowHeight, "Game", NULL, NULL);

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
	glViewport(0, 0, windowWidth, windowHeight);
	// Full screen viewport
	//glViewport(0, 0, mode->width, mode->height);
	//windowWidth = mode->width;
	//windowHeight = mode->height;

	// Register the callback function for when window gets resized
	glfwSetFramebufferSizeCallback(mWindow, FrameBufferSizeCallBack);

	// Enable v-sync by default
	glfwSwapInterval(1);

	// Enable z-buffering (depth testing)
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable face culling
	glEnable(GL_CULL_FACE);

	//// Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::vec3 lightPosition(1.0f, 0.0f, 1.0f);

	AssetManager* am = AssetManager::Get();

	mCamera = new Camera();
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	Texture* texture = new Texture("Assets/matrix.jpg");
	texture->SetType(TextureType::Emission);
	Texture* texture3 = new Texture("Assets/container2.png");
	Texture* texture4 = new Texture("Assets/container2_specular.png");
	texture4->SetType(TextureType::Specular);

	am->SaveTexture("Assets/matrix.jpg", texture);
	am->SaveTexture("Assets/container2.png", texture3);
	am->SaveTexture("Assets/container2_specular.png", texture4);

	DirectionalLight* dirLight = AllocateDirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f));

	Shader* colorShader = new Shader("Shaders/colorVS.glsl", "Shaders/colorFS.glsl");
	am->SaveShader("color", colorShader);

	Shader* phongShader = new Shader("Shaders/phongVS.glsl", "Shaders/phongFS.glsl");
	am->SaveShader("phong", phongShader);

	// General purpose material for objects colored with their vertices
	Material* colorMaterial = new Material({ glm::vec4(1.0f,1.0f,1.0f,1.0f), glm::vec4(1.0f,1.0f,1.0f,1.0f), 0.0f, 0.0f, false, false, false });
	colorMaterial->SetShader(colorShader);
	am->SaveMaterial("color", colorMaterial);

	//Shader* invertedColorShader = new Shader("Shaders/screenVS.glsl", "Shaders/Postprocess/invertedColorFS.glsl");
	//am->SaveShader("invertedColor", invertedColorShader);

	//Shader* grayScaleShader = new Shader("Shaders/screenVS.glsl", "Shaders/Postprocess/grayScaleFS.glsl");
	//am->SaveShader("grayScale", grayScaleShader);

	//Shader* sharpenKernelShader = new Shader("Shaders/screenVS.glsl", "Shaders/Postprocess/sharpenKernelFS.glsl");
	//am->SaveShader("sharpenKernel", sharpenKernelShader);

	//Shader* blurKernelShader = new Shader("Shaders/screenVS.glsl", "Shaders/Postprocess/blurKernelFS.glsl");
	//am->SaveShader("blurKernel", blurKernelShader);

	//Shader* edgeDetectKernelShader = new Shader("Shaders/screenVS.glsl", "Shaders/Postprocess/edgeDetectKernelFS.glsl");
	//am->SaveShader("edgeDetectKernel", edgeDetectKernelShader);

	Shader* screenShader = new Shader("Shaders/screenVS.glsl", "Shaders/screenFS.glsl");
	am->SaveShader("screen", screenShader);

	mFrameBuffer = new FrameBuffer(windowWidth, windowHeight);
	mFrameBuffer->SetShader(screenShader);

	// Skybox
	std::vector<std::string> faceNames
	{
		"Assets/skyboxes/skybox1/right.jpg",
		"Assets/skyboxes/skybox1/left.jpg",
		"Assets/skyboxes/skybox1/top.jpg",
		"Assets/skyboxes/skybox1/bottom.jpg",
		"Assets/skyboxes/skybox1/front.jpg",
		"Assets/skyboxes/skybox1/back.jpg"
	};
	mSkybox = new Skybox(faceNames);

	//Shader* skyboxShader = mSkybox->GetShader();


	CubeMap* sky = mSkybox->GetCubeMap();

	Shader* reflectiveShader = new Shader("Shaders/EnvironmentMapping/environmentMapVS.glsl", "Shaders/EnvironmentMapping/reflectionFS.glsl");
	reflectiveShader->SetActive();
	reflectiveShader->SetInt("cubeMap", static_cast<int>(TextureUnit::CubeMap));
	am->SaveShader("reflection", reflectiveShader);

	Material* reflectiveMat = new Material();
	reflectiveMat->SetShader(reflectiveShader);
	sky->SetActive(reflectiveShader);
	am->SaveMaterial("reflection", reflectiveMat);

	Shader* refractiveShader = new Shader("Shaders/EnvironmentMapping/environmentMapVS.glsl", "Shaders/EnvironmentMapping/refractionFS.glsl");
	refractiveShader->SetActive();
	refractiveShader->SetInt("cubeMap", static_cast<int>(TextureUnit::CubeMap));
	am->SaveShader("refraction", refractiveShader);

	Material* refractiveMat = new Material();
	refractiveMat->SetShader(refractiveShader);
	sky->SetActive(refractiveShader);
	am->SaveMaterial("refraction", refractiveMat);

	glUseProgram(0);

	Entity3D* sponza = new Entity3D("Assets/models/Sponza/sponza.obj");
	sponza->SetPosition(glm::vec3(0.0f, -5.0, 0.0f));
	sponza->SetScale(0.15);
	sponza->SetYaw(-90.0f);
	AddGameEntity(sponza);

	Cube* mCube = new Cube();
	mCube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	mCube->SetMaterial(new Material(*reflectiveMat));
	AddGameEntity(mCube);

	//Cube* object = new Cube();
	//object->SetPosition(glm::vec3(0.0f));
	//object->SetScale(0.5f);
	//Material* mat = new Material(*am->LoadMaterial("textured"));
	//mat->SetSpecularIntensity(5.0f);
	//mat->AddTexture(texture3);
	//mat->AddTexture(texture4);
	//mat->AddTexture(texture);
	//object->SetMaterial(mat);
	//object->SetYaw(25.0f);
	//TimerComponent* timer = new TimerComponent(object);
	//AddGameEntity(object);

	Entity3D* squidward = new Entity3D("Assets/models/Squidward/squidward.obj");
	squidward->SetPosition(glm::vec3(0.0f, -5.0f, 0.0f));
	squidward->SetScale(0.5f);
	//squidward->SetMaterialShader("tt", refractiveShader);
	Material* m = squidward->GetMaterial("tt");
	m->AddTexture(texture);
	m->SetSpecularIntensity(0.0f);
	AddGameEntity(squidward);

	// Configure uniform buffer object
	// Get buffer size
	size_t camBufferSize = sizeof(CameraBuffer);
	// Create the uniform buffer
	glGenBuffers(1, &uboCamera);
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferData(GL_UNIFORM_BUFFER, camBufferSize, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// Define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboCamera, 0, camBufferSize);

	// Get the relevant block indices
	unsigned int uniformBlockIndexPhong = glGetUniformBlockIndex(phongShader->GetID(), "CameraBuffer");
	unsigned int uniformBlockIndexColor = glGetUniformBlockIndex(colorShader->GetID(), "CameraBuffer");
	unsigned int uniformBlockIndexReflect = glGetUniformBlockIndex(reflectiveShader->GetID(), "CamerBuffer");
	unsigned int uniformBlockIndexRefract = glGetUniformBlockIndex(refractiveShader->GetID(), "CameraBuffer");
	// Link shader's uniform block to this uniform binding point 1
	glUniformBlockBinding(phongShader->GetID(), uniformBlockIndexPhong, 0);
	glUniformBlockBinding(colorShader->GetID(), uniformBlockIndexColor, 0);
	glUniformBlockBinding(reflectiveShader->GetID(), uniformBlockIndexReflect, 0);
	glUniformBlockBinding(refractiveShader->GetID(), uniformBlockIndexRefract, 0);

	// Store the view * projection matrix and camera position
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraBuffer), &mCamera->GetCameraConsts());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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

	delete mFrameBuffer;

	delete mSkybox;

	DeAllocateLights();
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
	float speed = 50.0f;

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

		AssetManager* am = AssetManager::Get();

		Entity3D* e = static_cast<Entity3D*>(mEntities[0]);
		Material* mat = e->GetMaterial("roof");
		Shader* reflect = am->LoadShader("reflection");
		Shader* phong = am->LoadShader("phong");

		if (mat->GetShader() == phong)
		{
			e->SetMaterialShader("roof", reflect);
		}
		else
		{
			e->SetMaterialShader("roof", phong);
		}

		//mLightArrays.mDirectionalLights[0]->SetIsEnabled(!mLightArrays.mDirectionalLights[0]->IsEnabled());
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		mPrevInputs[GLFW_KEY_SPACE] = false;
	}
}

void Game::Update(float deltaTime)
{
	for (size_t i = 0; i < MAX_LIGHTS; ++i)
	{
		if (mLightArrays.mPointLights[i])
		{
			mLightArrays.mPointLights[i]->SetLight();
		}
		if (mLightArrays.mDirectionalLights[i]) 
		{
			mLightArrays.mDirectionalLights[i]->SetLight();
		}
		if (mLightArrays.mSpotLights[i])
		{
			mLightArrays.mSpotLights[i]->SetLight();
		}
	}

	for (auto e : mEntities)
	{
		e->Update(deltaTime);
	}
}

void Game::Render()
{
	mCamera->SetActive(projection);

	// Store the view * projection matrix and camera position in uniform buffer
	glBindBuffer(GL_UNIFORM_BUFFER, uboCamera);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraBuffer), &mCamera->GetCameraConsts());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Uncomment this to draw to offscreen frame buffer instead
	mFrameBuffer->SetActive();

	// Specify color to clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// Clear the color buffer, depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto e : mEntities)
	{
		e->Draw();
	}

	// Draw sky box last
	mSkybox->Draw(mCamera->GetViewMatrix(), projection);

	// Uncomment this if using off screen frame buffer
	mFrameBuffer->End(windowWidth, windowHeight);

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
	projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);

	windowWidth = width;
	windowHeight = height;
}

PointLight* Game::AllocatePointLight(const glm::vec4& color, const glm::vec3& position, float constant, float linear, float quadratic)
{
	for (unsigned int i = 0; i < MAX_LIGHTS; ++i)
	{
		if (mLightArrays.mPointLights[i] == nullptr)
		{
			PointLight* pointLight = new PointLight(color, position, constant, linear, quadratic);
			pointLight->SetIsEnabled(true);
			pointLight->mIndex = i;
			mLightArrays.mPointLights[i] = pointLight;

			return pointLight;
		}
	}

	return nullptr;
}

DirectionalLight* Game::AllocateDirectionalLight(const glm::vec4& color, const glm::vec3& direction)
{
	for (unsigned int i = 0; i < MAX_LIGHTS; ++i)
	{
		if (mLightArrays.mDirectionalLights[i] == nullptr)
		{
			DirectionalLight* directionalLight = new DirectionalLight(direction);
			directionalLight->SetIsEnabled(true);
			directionalLight->mIndex = i;
			mLightArrays.mDirectionalLights[i] = directionalLight;

			return directionalLight;
		}
	}

	return nullptr;
}

SpotLight* Game::AllocateSpotLight(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& dir, float cutoff, float outerCutoff, float constant, float linear, float quadratic)
{
	for (unsigned int i = 0; i < MAX_LIGHTS; ++i)
	{
		if (mLightArrays.mSpotLights[i] == nullptr)
		{
			SpotLight* spotLight = new SpotLight(color, pos, dir, cutoff, outerCutoff, constant, linear, quadratic);
			spotLight->SetIsEnabled(true);
			spotLight->mIndex = i;
			mLightArrays.mSpotLights[i] = spotLight;

			return spotLight;
		}
	}

	return nullptr;
}

void Game::DeAllocateLights()
{
	for (unsigned int i = 0; i < MAX_LIGHTS; ++i)
	{
		delete mLightArrays.mPointLights[i];
		mLightArrays.mPointLights[i] = nullptr;

		delete mLightArrays.mDirectionalLights[i];
		mLightArrays.mDirectionalLights[i] = nullptr;

		delete mLightArrays.mSpotLights[i];
		mLightArrays.mSpotLights[i] = nullptr;
	}
}
