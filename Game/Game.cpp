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
#include "Plane.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "Mesh.h"
#include "UniformBuffer.h"
#include "Lights.h"
#include "MaterialCubeMap.h"
#include "ShadowMap.h"

int windowWidth = 1280;
int windowHeight = 720;

int subsamples = 4;

float nearPlane = 0.1f;
float farPlane = 10000.0f;

// projection matrix
glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), nearPlane, farPlane);

Game::Game() :
	mWindow(nullptr),
	mAssetManager(nullptr),
	mCamera(nullptr),
	mFrameBuffer(nullptr),
	mSkybox(nullptr),
	mLights(nullptr),
	mShadowMap(nullptr),
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

	glfwWindowHint(GLFW_SAMPLES, subsamples);

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

	// Enable anti-aliasing
	glEnable(GL_MULTISAMPLE);

	//// Enable blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glm::vec3 lightPosition(1.0f, 10.0f, 3.0f);

	mAssetManager = AssetManager::Get();

	mCamera = new Camera();
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	Texture* texture = new Texture("Assets/matrix.jpg", TextureType::Emission);
	Texture* texture3 = new Texture("Assets/container2.png", TextureType::Diffuse);
	Texture* texture4 = new Texture("Assets/container2_specular.png", TextureType::Specular);
	Texture* lightSphereTexture = new Texture("Assets/lightSphere.png", TextureType::Diffuse);
	Texture* woodTexture = new Texture("Assets/wood.png", TextureType::Diffuse);

	mAssetManager->SaveTexture("Assets/matrix.jpg", texture);
	mAssetManager->SaveTexture("Assets/container2.png", texture3);
	mAssetManager->SaveTexture("Assets/container2_specular.png", texture4);
	mAssetManager->SaveTexture("Assets/lightSphere.png", lightSphereTexture);
	mAssetManager->SaveTexture("Assets/wood.png", woodTexture);

	Shader* colorShader = new Shader("Shaders/colorVS.glsl", "Shaders/colorFS.glsl");
	mAssetManager->SaveShader("color", colorShader);

	Shader* phongShader = new Shader("Shaders/phongVS.glsl", "Shaders/phongFS.glsl");
	mAssetManager->SaveShader("phong", phongShader);

	Shader* textureShader = new Shader("Shaders/textureVS.glsl", "Shaders/textureFS.glsl");
	mAssetManager->SaveShader("texture", textureShader);

	Shader* instanceShader = new Shader("Shaders/instanceVS.glsl", "Shaders/instanceFS.glsl");
	mAssetManager->SaveShader("instance", instanceShader);

	UniformBuffer* materialBuffer = new UniformBuffer(sizeof(MaterialColors), BufferBindingPoint::Material, "MaterialBuffer");
	materialBuffer->LinkShader(phongShader);
	materialBuffer->LinkShader(instanceShader);
	materialBuffer->LinkShader(textureShader);
	mAssetManager->SaveBuffer("MaterialBuffer", materialBuffer);

	Material* lightSphereMaterial = new Material();
	lightSphereMaterial->SetShader(textureShader);
	lightSphereMaterial->AddTexture(lightSphereTexture);
	mAssetManager->SaveMaterial("lightSphere", lightSphereMaterial);

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
	mAssetManager->SaveShader("screen", screenShader);

	mFrameBuffer = new FrameBuffer(windowWidth, windowHeight, subsamples);
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

	CubeMap* sky = mSkybox->GetCubeMap();

	Shader* reflectiveShader = new Shader("Shaders/EnvironmentMapping/environmentMapVS.glsl", "Shaders/EnvironmentMapping/reflectionFS.glsl");
	reflectiveShader->SetActive();
	reflectiveShader->SetInt("cubeMap", static_cast<int>(TextureUnit::CubeMap));
	mAssetManager->SaveShader("reflection", reflectiveShader);

	MaterialCubeMap* reflectiveMat = new MaterialCubeMap();
	reflectiveMat->SetCubeMap(sky);
	reflectiveMat->SetShader(reflectiveShader);
	mAssetManager->SaveMaterial("reflection", reflectiveMat);

	Shader* refractiveShader = new Shader("Shaders/EnvironmentMapping/environmentMapVS.glsl", "Shaders/EnvironmentMapping/refractionFS.glsl");
	refractiveShader->SetActive();
	refractiveShader->SetInt("cubeMap", static_cast<int>(TextureUnit::CubeMap));
	mAssetManager->SaveShader("refraction", refractiveShader);

	MaterialCubeMap* refractiveMat = new MaterialCubeMap();
	refractiveMat->SetCubeMap(sky);
	refractiveMat->SetShader(refractiveShader);
	mAssetManager->SaveMaterial("refraction", refractiveMat);

	Shader* shadowDepthShader = new Shader("Shaders/shadowDepthVS.glsl", "Shaders/shadowDepthFS.glsl");
	mAssetManager->SaveShader("shadowDepth", shadowDepthShader);

	Shader* shadowDebugShader = new Shader("Shaders/screenVS.glsl", "Shaders/shadowDebugFS.glsl");
	mAssetManager->SaveShader("shadowDebug", shadowDebugShader);

	mShadowMap = new ShadowMap(lightPosition);
	mShadowMap->SetShader(shadowDepthShader);

	glUseProgram(0);

	Texture* rockTexture = new Texture("Assets/models/rock/rock.png", TextureType::Diffuse);
	mAssetManager->SaveTexture("rock", rockTexture);

	// Set model matrices for 10000 instances of a rock model
	unsigned int rockAmount = 10000;
	glm::mat4* rockMatrices;
	rockMatrices = new glm::mat4[rockAmount];
	std::srand(glfwGetTime()); // initialize random seed	
	float radius = 100.0;
	float offset = 20.5f;
	for (unsigned int i = 0; i < rockAmount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)rockAmount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y + 250.0f, z));

		// 2. scale: scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		rockMatrices[i] = model;
	}

	//Entity3D* rock = new Entity3D("Assets/models/rock/rock.obj");
	//rock->MakeInstance(rockAmount, rockMatrices);
	//Material* rockMat = rock->GetMaterial("Material");
	//rockMat->AddTexture(rockTexture);
	//rockMat->SetShader(instanceShader);
	//AddGameEntity(rock);

	delete[] rockMatrices;

	Entity3D* sponza = new Entity3D("Assets/models/Sponza/sponza.obj");
	sponza->SetPosition(glm::vec3(0.0f, -5.0, 0.0f));
	sponza->SetScale(0.15);
	sponza->SetYaw(-90.0f);
	AddGameEntity(sponza);

	Cube* mCube = new Cube();
	mCube->SetScale(2.0f);
	mCube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	mCube->SetMaterial(reflectiveMat);
	mCube->SetYaw(25.0f);
	TimerComponent* timer = new TimerComponent(mCube);
	AddGameEntity(mCube);

	Entity3D* squidward = new Entity3D("Assets/models/Squidward/squidward.obj");
	squidward->SetPosition(glm::vec3(0.0f, -5.0f, -15.0f));
	squidward->SetScale(0.5f);
	//squidward->SetMaterialShader("tt", refractiveShader);
	Material* m = squidward->GetMaterial("tt");
	m->AddTexture(texture);
	m->SetSpecularIntensity(0.0f);
	AddGameEntity(squidward);
	
	Material* woodMat = new Material();
	woodMat->AddTexture(woodTexture);
	woodMat->SetShader(phongShader);
	mAssetManager->SaveMaterial("woodMat", woodMat);

	Plane* plane = new Plane();
	plane->SetPitch(90.0f);
	plane->SetPosition(glm::vec3(0.0, -5.0f, 0.0f));
	plane->SetMaterial(woodMat);
	plane->SetScale(100.0f);
	AddGameEntity(plane);

	Cube* cube2 = new Cube();
	cube2->SetScale(3.0f);
	cube2->SetPosition(glm::vec3(5.0f, -3.0f, -7.0f));
	cube2->SetMaterial(woodMat);
	AddGameEntity(cube2);

	// Allocate lights in the scene
	mLights = new Lights();
	UniformBuffer* lightBuffer = mLights->GetLightBuffer();
	lightBuffer->LinkShader(phongShader);
	lightBuffer->LinkShader(instanceShader);

	DirectionalLight* dirLight = mLights->AllocateDirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f));

	PointLight* pointLight = mLights->AllocatePointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), lightPosition, 1.0f, 0.014f, 0.0007f);
	pointLight->data.specularIntensity = 3.0f;
	pointLight->data.usesShadow = true;
	Sphere* lightSphere = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSphere->SetMaterial(refractiveMat);
	lightSphere->SetPosition(lightPosition);
	AddGameEntity(lightSphere);

	//SpotLight* spotLight = mLights->AllocateSpotLight(glm::vec4(0.25f, 0.61f, 1.0f, 1.0f), glm::vec3(-0.7f, 3.0, 0.0f), glm::vec3(0.0, -1.0f, 0.0f),
	//	glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(16.0f)), 1.0f, 0.09f, 0.032f);
	//Sphere* lightSphere2 = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//lightSphere2->SetMaterial(reflectiveMat);
	//lightSphere2->SetPosition(glm::vec3(-0.7f, 3.0, 0.0f));
	//AddGameEntity(lightSphere2);

	PointLight* pointLight2 = mLights->AllocatePointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 3.0f, -170.0f), 1.0f, 0.014f, 0.0007f);
	pointLight2->data.specularIntensity = 3.0f;
	Sphere* lightSphere3 = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSphere3->SetMaterial(lightSphereMaterial);
	lightSphere3->SetPosition(glm::vec3(0.0f, 3.0f, -170.0f));
	AddGameEntity(lightSphere3);

	// Link shaders to camera's uniform buffer
	UniformBuffer* camBuffer = mCamera->GetCameraBuffer();
	camBuffer->LinkShader(phongShader);
	camBuffer->LinkShader(colorShader);
	camBuffer->LinkShader(reflectiveShader);
	camBuffer->LinkShader(refractiveShader);
	camBuffer->LinkShader(textureShader);
	camBuffer->LinkShader(instanceShader);
	camBuffer->UpdateBufferData(&mCamera->GetCameraConsts());

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

	delete mLights;

	delete mShadowMap;

	mAssetManager->Shutdown();
	mAssetManager = nullptr;
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

		//mLightArrays.spotLights[0].data.isEnabled = !mLightArrays.spotLights[0].data.isEnabled;
		//mLightArrays.pointLights[0].data.isEnabled = !mLightArrays.pointLights[0].data.isEnabled;
		//mLightArrays.pointLights[1].data.isEnabled = !mLightArrays.pointLights[1].data.isEnabled;

		mLights->GetLights().directionalLight[0].data.isEnabled = !mLights->GetLights().directionalLight[0].data.isEnabled;
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
	mCamera->SetActive(projection);

	mLights->SetActive();

	// Uncomment this to draw to offscreen frame buffer instead
	//mFrameBuffer->SetActive();

	// Specify color to clear the screen
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	// Clear the color buffer, depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	mShadowMap->SetActive();


	//RenderScene();
	RenderScene(mShadowMap->GetShader());


	//mShadowMap->End(windowWidth, windowHeight, mAssetManager->LoadShader("shadowDebug"));
	mShadowMap->End(windowWidth, windowHeight, mAssetManager->LoadShader("phong"));

	RenderScene();

	// Uncomment this if using off screen frame buffer
	//mFrameBuffer->End(windowWidth, windowHeight);

	glfwSwapBuffers(mWindow);
}

void Game::RenderScene()
{
	for (auto e : mEntities)
	{
		e->Draw();
	}

	// Draw sky box last
	mSkybox->Draw(mCamera->GetViewMatrix(), projection);
}

void Game::RenderScene(Shader* shader)
{
	for (auto e : mEntities)
	{
		static_cast<Entity3D*>(e)->Draw(shader);
	}

	mSkybox->Draw(mCamera->GetViewMatrix(), projection);
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
