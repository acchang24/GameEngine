#include "Game.h"
#include <chrono>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include "3dPrimitives/Cube.h"
#include "3dPrimitives/Plane.h"
#include "3dPrimitives/Sphere.h"
#include "Animation/Skeleton.h"
#include "Components/AnimationComponent.h"
#include "Entity/Entity3D.h"
#include "Graphics/Camera.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/FrameBufferMultiSampled.h"
#include "Graphics/Lights.h"
#include "Graphics/Material.h"
#include "Graphics/MaterialCubeMap.h"
#include "Graphics/Mesh.h"
#include "Graphics/Model.h"
#include "Graphics/Renderer3D.h"
#include "Graphics/Shader.h"
#include "Graphics/ShadowMap.h"
#include "Graphics/Skybox.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexLayouts.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"
#include "Profiler/Profiler.h"
#include "Util/Random.h"


float size = 250.0f;
float near = 1.0f;
float far = 750.0f;
glm::vec3 pos(0.0f);
glm::vec3 lightDir(-0.05f, -1.0f, -0.2f);
float dist = 650.0f;

bool IS_FULLSCREEN = false;
int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
double MOUSE_SENSITIVITY = 0.05;
int SUB_SAMPLES = 4;
int VSYNC = 1;
const char* TITLE = "Game";

Game::Game() :
	mPrevKeyInputs(),
	mRenderer(nullptr),
	mAssetManager(nullptr),
	mJobManager(nullptr),
	mCamera(nullptr),
	mSkybox(nullptr),
	mLights(nullptr),
	mShadowMap(nullptr),
	mMousePosX(0),
	mMousePosY(0),
	mIsRunning(true),
	hdr(false),
	bloom(false),
	mMouseCaptured(SDL_TRUE)
{
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	mJobManager = JobManager::Get();
	mJobManager->Begin();

	mRenderer = Renderer3D::Get();
	mRenderer->Init(WINDOW_WIDTH, WINDOW_HEIGHT, SUB_SAMPLES, VSYNC, IS_FULLSCREEN, mMouseCaptured, "Game");

	mAssetManager = AssetManager::Get();

	mCamera = new Camera();
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	PROFILE_SCOPE(LOAD_DATA);

	Texture* texture = new Texture("Assets/matrix.jpg", TextureType::Emission);
	Texture* texture3 = new Texture("Assets/container2.png", TextureType::Diffuse);
	Texture* texture4 = new Texture("Assets/container2_specular.png", TextureType::Specular);
	Texture* lightSphereTexture = new Texture("Assets/lightSphere.png", TextureType::Diffuse);
	Texture* woodTexture = new Texture("Assets/wood.png", TextureType::Diffuse);
	Texture* wallTexture = new Texture("Assets/brickwall.jpg", TextureType::Diffuse);
	Texture* wallNormalTexture = new Texture("Assets/brickwall_normal.jpg", TextureType::Normal);

	mAssetManager->SaveTexture("Assets/matrix.jpg", texture);
	mAssetManager->SaveTexture("Assets/container2.png", texture3);
	mAssetManager->SaveTexture("Assets/container2_specular.png", texture4);
	mAssetManager->SaveTexture("Assets/lightSphere.png", lightSphereTexture);
	mAssetManager->SaveTexture("Assets/wood.png", woodTexture);
	mAssetManager->SaveTexture("Assets/brickwall.jpg", wallTexture);
	mAssetManager->SaveTexture("Assets/brickwall_normal.jpg", wallNormalTexture);

	Shader* colorShader = new Shader("Shaders/color.vert", "Shaders/color.frag");
	mAssetManager->SaveShader("color", colorShader);

	Shader* phongShader = new Shader("Shaders/phong.vert", "Shaders/phong.frag");
	mAssetManager->SaveShader("phong", phongShader);

	Shader* skinnedShader = new Shader("Shaders/Animation/skinned.vert", "Shaders/phong.frag");
	mAssetManager->SaveShader("skinned", skinnedShader);

	Shader* textureShader = new Shader("Shaders/texture.vert", "Shaders/texture.frag");
	mAssetManager->SaveShader("texture", textureShader);

	Shader* instanceShader = new Shader("Shaders/instance.vert", "Shaders/phong.frag");
	mAssetManager->SaveShader("instance", instanceShader);

	Shader* shadowDepthShader = new Shader("Shaders/Shadow/shadowDepth.vert", "Shaders/Shadow/shadowDepth.frag");
	mAssetManager->SaveShader("shadowDepth", shadowDepthShader);

	Shader* shadowDebugShader = new Shader("Shaders/screen.vert", "Shaders/Shadow/shadowDebug.frag");
	mAssetManager->SaveShader("shadowDebug", shadowDebugShader);

	// Link shader uniform blocks for the material buffer
	mRenderer->LinkShaderToUniformBlock(mRenderer->GetMaterialBuffer(), phongShader);
	mRenderer->LinkShaderToUniformBlock(mRenderer->GetMaterialBuffer(), instanceShader);
	mRenderer->LinkShaderToUniformBlock(mRenderer->GetMaterialBuffer(), textureShader);

	// Link shader uniform blocks for the skeleton buffer
	mRenderer->LinkShaderToUniformBlock(mRenderer->GetSkeletonBuffer(), skinnedShader);
	mRenderer->LinkShaderToUniformBlock(mRenderer->GetSkeletonBuffer(), shadowDepthShader);

	Material* lightSphereMaterial = new Material();
	lightSphereMaterial->SetShader(textureShader);
	lightSphereMaterial->AddTexture(lightSphereTexture);
	mAssetManager->SaveMaterial("lightSphere", lightSphereMaterial);

	//Shader* invertedColorShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/invertedColor.frag");
	//mAssetManager->SaveShader("invertedColor", invertedColorShader);

	//Shader* grayScaleShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/grayScale.frag");
	//mAssetManager->SaveShader("grayScale", grayScaleShader);

	//Shader* sharpenKernelShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/sharpenKernel.frag");
	//mAssetManager->SaveShader("sharpenKernel", sharpenKernelShader);

	//Shader* blurKernelShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/blurKernel.frag");
	//mAssetManager->SaveShader("blurKernel", blurKernelShader);

	//Shader* edgeDetectKernelShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/edgeDetectKernel.frag");
	//mAssetManager->SaveShader("edgeDetectKernel", edgeDetectKernelShader);

	Shader* copyScreenShader = new Shader("Shaders/screen.vert", "Shaders/copyScreen.frag");
	mAssetManager->SaveShader("copyScreen", copyScreenShader);

	Shader* bloomMaskShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/Bloom/bloomMask.frag");
	mAssetManager->SaveShader("bloomMask", bloomMaskShader);

	Shader* bloomBlurHorizontalShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/Bloom/bloomBlurHorizontal.frag");
	bloomBlurHorizontalShader->SetActive();
	bloomBlurHorizontalShader->SetFloat("width", static_cast<float>(WINDOW_WIDTH / 4));
	mAssetManager->SaveShader("bloomBlurHorizontal", bloomBlurHorizontalShader);

	Shader* bloomBlurVerticalShader = new Shader("Shaders/screen.vert", "Shaders/Postprocess/Bloom/bloomBlurVertical.frag");
	bloomBlurVerticalShader->SetActive();
	bloomBlurVerticalShader->SetFloat("height", static_cast<float>(WINDOW_HEIGHT / 4));
	mAssetManager->SaveShader("bloomBlurVertical", bloomBlurVerticalShader);

	Shader* hdrGammaShader = new Shader("Shaders/screen.vert", "Shaders/hdrGamma.frag");
	hdrGammaShader->SetActive();
	hdrGammaShader->SetFloat("exposure", 1.0f);
	hdrGammaShader->SetBool("hdr", hdr);
	hdrGammaShader->SetBool("bloom", bloom);
	mAssetManager->SaveShader("hdrGamma", hdrGammaShader);

	mRenderer->SetFrameBufferShader(static_cast<FrameBufferMultiSampled*>(mRenderer->GetMainFrameBuffer()), hdrGammaShader);
	mRenderer->SetFrameBufferShader(mRenderer->GetBloomMaskFrameBuffer(), bloomMaskShader);
	mRenderer->SetFrameBufferShader(mRenderer->GetBloomBlurHorizontalFrameBuffer(), bloomBlurHorizontalShader);
	mRenderer->SetFrameBufferShader(mRenderer->GetBloomBlurVerticalFrameBuffer(), bloomBlurVerticalShader);

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

	Shader* reflectiveShader = new Shader("Shaders/EnvironmentMapping/environmentMap.vert", "Shaders/EnvironmentMapping/reflection.frag");
	reflectiveShader->SetActive();
	reflectiveShader->SetInt("cubeMap", static_cast<int>(TextureUnit::CubeMap));
	mAssetManager->SaveShader("reflection", reflectiveShader);

	MaterialCubeMap* reflectiveMat = new MaterialCubeMap();
	reflectiveMat->SetCubeMap(sky);
	reflectiveMat->SetShader(reflectiveShader);
	mAssetManager->SaveMaterial("reflection", reflectiveMat);

	Shader* refractiveShader = new Shader("Shaders/EnvironmentMapping/environmentMap.vert", "Shaders/EnvironmentMapping/refraction.frag");
	refractiveShader->SetActive();
	refractiveShader->SetInt("cubeMap", static_cast<int>(TextureUnit::CubeMap));
	mAssetManager->SaveShader("refraction", refractiveShader);

	MaterialCubeMap* refractiveMat = new MaterialCubeMap();
	refractiveMat->SetCubeMap(sky);
	refractiveMat->SetShader(refractiveShader);
	mAssetManager->SaveMaterial("refraction", refractiveMat);

	glUseProgram(0);

	//Texture* rockTexture = new Texture("Assets/models/rock/rock.png", TextureType::Diffuse);
	//mAssetManager->SaveTexture("rock", rockTexture);

	//// Set model matrices for 10000 instances of a rock model
	//unsigned int rockAmount = 10000;
	//glm::mat4* rockMatrices;
	//rockMatrices = new glm::mat4[rockAmount];
	//float radius = 100.0;
	//float offset = 20.5f;
	//for (unsigned int i = 0; i < rockAmount; i++)
	//{
	//	glm::mat4 model = glm::mat4(1.0f);
	//	// 1. translation: displace along circle with 'radius' in range [-offset, offset]
	//	float angle = (float)i / (float)rockAmount * 360.0f;
	//	float displacement = Random::GetFloatRange(-offset, offset);
	//	float x = sin(angle) * radius + displacement;
	//	displacement = Random::GetFloatRange(-offset, offset);
	//	float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
	//	displacement = Random::GetFloatRange(-offset, offset);
	//	float z = cos(angle) * radius + displacement;
	//	model = glm::translate(model, glm::vec3(x, y + 250.0f, z));

	//	// 2. scale: scale between 0.05 and 0.25f
	//	float scale = Random::GetFloatRange(0.05f, 0.25f);
	//	model = glm::scale(model, glm::vec3(scale));

	//	// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
	//	float rotAngle = Random::GetFloatRange(0.0f, 360.0f);
	//	model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

	//	// 4. now add to list of matrices
	//	rockMatrices[i] = model;
	//}

	//Entity3D* rock = new Entity3D("Assets/models/rock/rock.obj");
	//rock->MakeInstance(rockAmount, rockMatrices);
	//Material* rockMat = rock->GetModel()->GetMaterial("Material");
	//rockMat->AddTexture(rockTexture);
	//rockMat->SetShader(instanceShader);
	//AddGameEntity(rock);

	//delete[] rockMatrices;

	std::vector<glm::vec3> vampirePositions = { 
		glm::vec3(0.0f, -4.0f, 0.0f), glm::vec3(10.0f, -4.0f, 0.0f),
		glm::vec3(-10.0f, -4.0f, 0.0f), glm::vec3(10.0f, -4.0f, 10.0f),
		glm::vec3(10.0f, -4.0f, -10.0f), glm::vec3(20.0f, -4.0f, 0.0f),
		glm::vec3(-15.0f, -4.0f, 0.0f), glm::vec3(15.0f, -4.0f, 0.0f),
		glm::vec3(10.0f, -4.0f, 15.0f), glm::vec3(10.0f, -4.0f, -15.0f),
		glm::vec3(10.0f, -4.0f, -15.0f) 
	};

	for (size_t i = 0; i < 11; ++i)
	{
		Entity3D* vampire = new Entity3D("Assets/models/vampire/dancing_vampire.dae");
		vampire->SetScale(0.05f);
		vampire->SetPosition(vampirePositions[i]);
		AddGameEntity(vampire);
	}

	Entity3D* sponza = new Entity3D("Assets/models/Sponza/sponza.obj");
	sponza->SetPosition(glm::vec3(0.0f, -5.0, 0.0f));
	sponza->SetScale(0.1);
	sponza->SetYaw(-90.0f);
	AddGameEntity(sponza);

	//Material* wallMaterial = new Material();
	//wallMaterial->SetShader(phongShader);
	//wallMaterial->AddTexture(wallTexture);
	//wallMaterial->AddTexture(wallNormalTexture);
	//mAssetManager->SaveMaterial("wall", wallMaterial);

	//Plane* wall = new Plane();
	////wall->SetPitch(-90.0f);
	//wall->SetPosition(glm::vec3(0.0f, -3.0f, 7.0f));
	//wall->SetMaterial(wallMaterial);
	//AddGameEntity(wall);

	//Cube* mCube = new Cube();
	//mCube->SetScale(2.0f);
	//mCube->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//mCube->SetMaterial(reflectiveMat);
	//mCube->SetYaw(25.0f);
	//AddGameEntity(mCube);

	Entity3D* squidward = new Entity3D("Assets/models/SquidwardDance/Rumba Dancing.dae");
	squidward->SetPosition(glm::vec3(0.0f, -5.0f, -15.0f));
	squidward->SetScale(0.35f);
	squidward->SetYaw(-90.0f);
	squidward->SetPitch(45.0f);
	squidward->SetRoll(20.0f);
	glm::quat rot = squidward->GetQuatRotation();
	glm::vec3 euler = glm::eulerAngles(rot);

	std::cout << "pitch: " << glm::degrees(euler.x) << " yaw: " << glm::degrees(euler.y) << " roll: " << glm::degrees(euler.z) << "\n";
	glm::quat rot2 = glm::quat(euler);

	Entity3D* squidward2 = new Entity3D("Assets/models/SquidwardDance/Rumba Dancing.dae");
	squidward2->SetPosition(glm::vec3(10.0f, -5.0f, -15.0f)); 
	squidward2->SetScale(0.35f);
	glm::quat newRot = glm::quat(glm::vec3(glm::radians(45.0f), glm::radians(-90.0f), glm::radians(20.0f)));
	squidward2->SetQuatRotation(newRot);
	AddGameEntity(squidward2);

	glm::quat rot3 = squidward2->GetQuatRotation();
	glm::vec3 euler2 = glm::eulerAngles(rot3);
	std::cout << "pitch: " << glm::degrees(euler2.x) << " yaw: " << glm::degrees(euler2.y) << " roll: " << glm::degrees(euler2.z) << "\n";
	glm::vec3 euler3 = glm::eulerAngles(newRot);
	std::cout << "pitch: " << glm::degrees(euler3.x) << " yaw: " << glm::degrees(euler3.y) << " roll: " << glm::degrees(euler3.z) << "\n";
	glm::quat testquat = glm::quat(euler2);

	//squidward->SetMaterialShader("tt", refractiveShader);
	Material* m = squidward->GetModel()->GetMaterial("ttmat");
	//m->AddTexture(texture);
	m->SetSpecularIntensity(0.0f);
	AddGameEntity(squidward);
	
	//Material* woodMat = new Material();
	//woodMat->AddTexture(woodTexture);
	//woodMat->SetShader(phongShader);
	//mAssetManager->SaveMaterial("woodMat", woodMat);

	//Plane* plane = new Plane();
	//plane->SetPitch(-90.0f);
	//plane->SetPosition(glm::vec3(0.0, -5.0f, 0.0f));
	//plane->SetMaterial(woodMat);
	//plane->SetScale(70.0f);
	//AddGameEntity(plane);

	//Cube* cube2 = new Cube();
	//cube2->SetScale(3.0f);
	//cube2->SetPosition(glm::vec3(5.0f, -3.0f, -7.0f));
	//cube2->SetMaterial(woodMat);
	//AddGameEntity(cube2);

	
	//glm::vec3 lightPosition(1.0f, 10.0f, 3.0f);
	glm::vec3 lightPosition = lightDir * -dist;
	mShadowMap = new ShadowMap(lightPosition);
	mShadowMap->SetShader(shadowDepthShader);
	pos = lightPosition;

	// Allocate lights in the scene
	mLights = new Lights();
	UniformBuffer* lightBuffer = mLights->GetLightBuffer();
	lightBuffer->LinkShader(phongShader);
	lightBuffer->LinkShader(instanceShader);

	DirectionalLight* dirLight = mLights->AllocateDirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	dirLight->data.usesShadow = true;

	PointLight* pointLight = mLights->AllocatePointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 3.0f, 60.0f), 1.0f, 0.014f, 0.0007f);
	pointLight->data.diffuseIntensity = 70.0f;
	pointLight->data.specularIntensity = 50.0f;
	Sphere* lightSphere = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSphere->SetMaterial(lightSphereMaterial);
	lightSphere->SetPosition(glm::vec3(1.0f, 3.0f, 60.0f));
	AddGameEntity(lightSphere);

	//SpotLight* spotLight = mLights->AllocateSpotLight(glm::vec4(0.25f, 0.61f, 1.0f, 1.0f), glm::vec3(-0.7f, 3.0, 0.0f), glm::vec3(0.0, -1.0f, 0.0f),
	//	glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(16.0f)), 1.0f, 0.09f, 0.032f);
	//Sphere* lightSphere2 = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//lightSphere2->SetMaterial(reflectiveMat);
	//lightSphere2->SetPosition(glm::vec3(-0.7f, 3.0, 0.0f));
	//AddGameEntity(lightSphere2);

	PointLight* pointLight2 = mLights->AllocatePointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 3.0f, -120.0f), 1.0f, 0.014f, 0.0007f);
	pointLight2->data.diffuseIntensity = 70.0f;
	pointLight2->data.specularIntensity = 900.0f;
	Sphere* lightSphere3 = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSphere3->SetMaterial(lightSphereMaterial);
	lightSphere3->SetPosition(glm::vec3(0.0f, 3.0f, -120.0f));
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
	mRenderer->Shutdown();
	mRenderer = nullptr;

	UnloadGameData();

	delete mCamera;

	delete mSkybox;

	delete mLights;

	delete mShadowMap;

	mJobManager->End();
	mJobManager = nullptr;

	mAssetManager->Shutdown();
	mAssetManager = nullptr;
}

bool Game::LoadGameData()
{
	return true;
}

void Game::UnloadGameData()
{
	for (auto e : mEntities)
	{
		delete e;
	}
	mEntities.clear();
}

void Game::Run()
{
	std::chrono::high_resolution_clock::time_point frameStart = std::chrono::high_resolution_clock::now();

	while (mIsRunning)
	{
		Profiler::Get()->ResetAll();

		PROFILE_SCOPE(GAME_LOOP);

		// Calculate delta time
		std::chrono::high_resolution_clock::time_point frameEnd = std::chrono::high_resolution_clock::now();
		double duration = static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart).count());
		float deltaTime = static_cast<float>(0.000000001 * duration);
		frameStart = frameEnd;

		ProcessInput();

		Update(deltaTime);

		Render();
	}
}

void Game::ProcessInput()
{
	Uint8 mouseButtonDown = 0;
	Uint8 mouseButtonUp = 0;
	Sint32 scrollDir = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			// Check to see when user quits the game
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			// Mouse click down
			mouseButtonDown = event.button.button;
			break;
		case SDL_MOUSEBUTTONUP:
			// Mouse click release
			mouseButtonUp = event.button.button;
			break;
		case SDL_MOUSEWHEEL:
			// Mouse wheel scroll
			scrollDir = event.wheel.y;
			break;
		}
	}

	ProcessMouseInput(mouseButtonDown, mouseButtonUp, scrollDir);

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	// Camera
	CameraMode mode = mCamera->GetCameraMode();
	glm::vec3 right = mCamera->GetRight();
	glm::vec3 up = mCamera->GetUp();

	glm::vec3 camPanDir = glm::vec3(0.0f, 0.0f, 0.0f);

	// Move camera around
	if (keyboardState[SDL_SCANCODE_W])
	{	
		switch (mode)
		{
		case CameraMode::First:
			camPanDir += glm::normalize(glm::cross(up, right));
			break;
		case CameraMode::Fly:
			camPanDir += mCamera->GetForward();
			break;
		}
	}
	if (keyboardState[SDL_SCANCODE_S])
	{
		switch (mode)
		{
		case CameraMode::First:
			camPanDir -= glm::normalize(glm::cross(up, right));
			break;
		case CameraMode::Fly:
			camPanDir -= mCamera->GetForward();
			break;
		}
	}
	if (keyboardState[SDL_SCANCODE_A])
	{
		if (mode == CameraMode::First || mode == CameraMode::Fly)
		{
			camPanDir -= right;
		}
	}
	if (keyboardState[SDL_SCANCODE_D])
	{
		if (mode == CameraMode::First || mode == CameraMode::Fly)
		{
			camPanDir += right;
		}
	}
	mCamera->SetPanDir(camPanDir);

	// Toggle the main directional light
	if (keyboardState[SDL_SCANCODE_SPACE] && !mPrevKeyInputs[SDL_SCANCODE_SPACE])
	{
		mLights->GetLights().directionalLight[0].data.isEnabled = !mLights->GetLights().directionalLight[0].data.isEnabled;
	}

	// Capture mouse
	if (keyboardState[SDL_SCANCODE_M] && !mPrevKeyInputs[SDL_SCANCODE_M])
	{
		if (mMouseCaptured == SDL_TRUE)
		{
			mMouseCaptured = SDL_FALSE;
			mMousePosX = 0;
			mMousePosY = 0;
		}
		else
		{
			mMouseCaptured = SDL_TRUE;
		}

		// Enable relative mouse mode
		SDL_SetRelativeMouseMode(mMouseCaptured);
		// Clear any saved values
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}


	// HDR/Exposure
	Shader* shader = mAssetManager->LoadShader("hdrGamma");

	// Toggle hdr
	if (keyboardState[SDL_SCANCODE_H] && !mPrevKeyInputs[SDL_SCANCODE_H])
	{
		hdr = !hdr;
		shader->SetActive();
		shader->SetBool("hdr", hdr);
	}
	// Toggle bloom
	if (keyboardState[SDL_SCANCODE_B] && !mPrevKeyInputs[SDL_SCANCODE_B])
	{
		bloom = !bloom;

		shader->SetActive();
		shader->SetBool("bloom", bloom);
	}
	// Exposure levels
	if (keyboardState[SDL_SCANCODE_0])
	{
		shader->SetActive();
		shader->SetFloat("exposure", 0.1f);
	}
	if (keyboardState[SDL_SCANCODE_1])
	{
		shader->SetActive();
		shader->SetFloat("exposure", 1.0f);
	}
	if (keyboardState[SDL_SCANCODE_2])
	{
		shader->SetActive();
		shader->SetFloat("exposure", 2.0f);
	}
	if (keyboardState[SDL_SCANCODE_3])
	{
		shader->SetActive();
		shader->SetFloat("exposure", 3.0f);
	}
	if (keyboardState[SDL_SCANCODE_4])
	{
		shader->SetActive();
		shader->SetFloat("exposure", 4.0f);
	}
	if (keyboardState[SDL_SCANCODE_5])
	{
		shader->SetActive();
		shader->SetFloat("exposure", 5.0f);
	}

	// Shadow debug inputs
	if (keyboardState[SDL_SCANCODE_UP] && !mPrevKeyInputs[SDL_SCANCODE_UP])
	{
		size += 10.0f;
	}
	if (keyboardState[SDL_SCANCODE_DOWN] && !mPrevKeyInputs[SDL_SCANCODE_DOWN])
	{
		size -= 10.0f;
	}
	if (keyboardState[SDL_SCANCODE_LEFT] && !mPrevKeyInputs[SDL_SCANCODE_LEFT])
	{
		far -= 10.0f;
	}
	if (keyboardState[SDL_SCANCODE_RIGHT] && !mPrevKeyInputs[SDL_SCANCODE_RIGHT])
	{
		far += 10.0f;
	}
	if (keyboardState[SDL_SCANCODE_L] && !mPrevKeyInputs[SDL_SCANCODE_L])
	{
		dist -= 10.0f;
		pos = (lightDir * -dist);
	}
	if (keyboardState[SDL_SCANCODE_K] && !mPrevKeyInputs[SDL_SCANCODE_K])
	{
		dist += 10.0f;
		pos = (lightDir * -dist);
	}

	// Save previous key inputs
	mPrevKeyInputs[SDL_SCANCODE_SPACE] = keyboardState[SDL_SCANCODE_SPACE];
	mPrevKeyInputs[SDL_SCANCODE_H] = keyboardState[SDL_SCANCODE_H];
	mPrevKeyInputs[SDL_SCANCODE_B] = keyboardState[SDL_SCANCODE_B];
	mPrevKeyInputs[SDL_SCANCODE_UP] = keyboardState[SDL_SCANCODE_UP];
	mPrevKeyInputs[SDL_SCANCODE_DOWN] = keyboardState[SDL_SCANCODE_DOWN];
	mPrevKeyInputs[SDL_SCANCODE_LEFT] = keyboardState[SDL_SCANCODE_LEFT];
	mPrevKeyInputs[SDL_SCANCODE_RIGHT] = keyboardState[SDL_SCANCODE_RIGHT];
	mPrevKeyInputs[SDL_SCANCODE_L] = keyboardState[SDL_SCANCODE_L];
	mPrevKeyInputs[SDL_SCANCODE_K] = keyboardState[SDL_SCANCODE_K];
	mPrevKeyInputs[SDL_SCANCODE_M] = keyboardState[SDL_SCANCODE_M];
}

void Game::ProcessMouseInput(Uint8 buttonDown, Uint8 buttonUp, Sint32 scroll)
{
	// Single left click
	if (buttonDown == SDL_BUTTON_LEFT)
	{
		std::cout << "Left Click\n";
	}
	// Single right click
	if (buttonDown == SDL_BUTTON_RIGHT)
	{
		std::cout << "Right Click\n";
	}
	// Single scroll wheel click (mouse 3 click)
	if (buttonDown == SDL_BUTTON_MIDDLE)
	{
		std::cout << "Scroll Click\n";
	}
	// Single mouse 4 click
	if (buttonDown == SDL_BUTTON_X1)
	{
		std::cout << "Mouse 4 Click\n";
	}
	// Single mouse 5 click
	if (buttonDown == SDL_BUTTON_X2)
	{
		std::cout << "Mouse 5 Click\n";
	}

	Uint8 mouse_state = SDL_GetMouseState(NULL, NULL);
	// Left click hold
	if (mouse_state & SDL_BUTTON_LMASK)
	{
		std::cout << "Left Click hold\n";
	}
	// Right click hold
	if (mouse_state & SDL_BUTTON_RMASK)
	{
		std::cout << "Right Click hold\n";
	}
	// Scroll click hold
	if (mouse_state & SDL_BUTTON_MMASK)
	{
		std::cout << "Scroll Click hold\n";
	}
	// Mouse 4 click hold
	if (mouse_state & SDL_BUTTON_X1MASK)
	{
		std::cout << "Mouse 4 Click hold\n";
	}
	// Mouse 5 click hold
	if (mouse_state & SDL_BUTTON_X2MASK)
	{
		std::cout << "Mouse 5 Click hold\n";
	}

	// Left click release
	if (buttonUp == SDL_BUTTON_LEFT)
	{
		std::cout << "Left Click Release\n";
	}
	// Right click release
	if (buttonUp == SDL_BUTTON_RIGHT)
	{
		std::cout << "Right Click Release\n";
	}
	// Scroll click release
	if (buttonUp == SDL_BUTTON_MIDDLE)
	{
		std::cout << "Scroll Click Release\n";
	}
	// Mouse 4 click release
	if (buttonUp == SDL_BUTTON_X1)
	{
		std::cout << "Mouse 4 Click Release\n";
	}
	// Mouse 5 click release
	if (buttonUp == SDL_BUTTON_X2)
	{
		std::cout << "Mouse 5 Click Release\n";
	}

	// Scroll wheel up
	if (scroll >= 1)
	{
		std::cout << "Scroll Up\n";
	}
	// Scroll wheel down
	if (scroll <= -1)
	{
		std::cout << "Scroll Down\n";
	}

	if (mMouseCaptured == SDL_TRUE)
	{
		// Calculate mouse movement
		int x = 0;
		int y = 0;
		SDL_GetRelativeMouseState(&x, &y);
		mMousePosX = x * MOUSE_SENSITIVITY;
		mMousePosY = -y * MOUSE_SENSITIVITY;
	}
}

void Game::Update(float deltaTime)
{
	PROFILE_SCOPE(UPDATE);

	for (auto e : mEntities)
	{
		e->Update(deltaTime);
	}

	mCamera->Update(deltaTime, mMousePosX, mMousePosY);

	//mJobManager->WaitForJobs();
}

void Game::Render()
{
	PROFILE_SCOPE(RENDER);

	mCamera->SetActive();

	mLights->SetActive();

	mRenderer->ClearBuffers();

	mRenderer->SetFrameBuffer();
	{
		//PROFILE_SCOPE(RENDER_SHADOW_MAP);

		//std::cout << size << " " << near << " " << far << " " << pos.x << " " << pos.y << " " << pos.z << "\n";

		// Render to shadow map
		//mShadowMap->SetActive(size, near, far, pos);
		//RenderScene(mShadowMap->GetShader());

		// Render the shadow map
		//mShadowMap->End(windowWidth, windowHeight, mAssetManager->LoadShader("shadowDebug"));
		//mShadowMap->DrawDebug(mAssetManager->LoadShader("shadowDebug"));

		// End shadow render pass
		//mShadowMap->End(windowWidth, windowHeight, mAssetManager->LoadShader("phong"));
	}
	{
		////PROFILE_SCOPE(RENDER_NORMAL_SCENE);
		//// Render scene as normal
		RenderScene();

	}
	mRenderer->DrawFrameBuffers();
	//mShadowMap->DrawDebug(mAssetManager->LoadShader("shadowDebug"));
	//glViewport(0, 0, windowWidth, windowHeight);

	mRenderer->EndFrame();
}

void Game::RenderScene()
{
	PROFILE_SCOPE(RENDER_SCENE_NORMAL);

	for (auto e : mEntities)
	{
		e->Draw();
	}

	mSkybox->Draw(mCamera->GetViewMatrix(), mCamera->GetProjectionMatrix());
}

void Game::RenderScene(Shader* shader)
{
	for (auto e : mEntities)
	{
		static_cast<Entity3D*>(e)->Draw(shader);
	}

	mSkybox->Draw(mCamera->GetViewMatrix(), mCamera->GetProjectionMatrix());
}

void Game::RemoveGameEntity(Entity* e)
{
	auto iter = std::find(mEntities.begin(), mEntities.end(), e);
	if (iter != mEntities.end())
	{
		// Swap to end of vector and pop off
		auto iter2 = mEntities.end() - 1;
		std::iter_swap(iter, iter2);
		delete e;
		mEntities.pop_back();
	}
}
