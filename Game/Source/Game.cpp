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
#include "Components/AnimationComponent.h"
#include "Entity/Entity3D.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/FrameBufferMultiSampled.h"
#include "Graphics/Material.h"
#include "Graphics/MaterialCubeMap.h"
#include "Graphics/Mesh.h"
#include "Graphics/Model.h"
#include "Graphics/Renderer.h"
#include "Graphics/Shader.h"
#include "Graphics/ShadowMap.h"
#include "Graphics/Skybox.h"
#include "Graphics/Texture.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/VertexLayouts.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "MemoryManager/AssetManager.h"
#include "Multithreading/JobManager.h"
#include "Util/Console.h"
#include "Util/LoggerMacros.h"
#include "Util/Profiler.h"
#include "Util/Random.h"


float size = 30.0f;
float near = 1.0f;
float far = 40.0f;
glm::vec3 pos(0.0f);
glm::vec3 lightDir(-0.05f, -1.0f, -0.2f);
float dist = 10.0f;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
int SUB_SAMPLES = 4;
int VSYNC = 1;
double MOUSE_SENSITIVITY = 0.05;
bool IS_FULLSCREEN = false;
const char* TITLE = "Game";
SDL_bool MOUSE_CAPTURED = SDL_TRUE;

Game::Game() :
	mEngine(RendererMode::MODE_3D, MOUSE_SENSITIVITY),
	mCamera(nullptr),
	mLights(),
	mShadowMap(nullptr),
	mSkybox(nullptr),
	mMainFrameBuffer(nullptr),
	mBloomMaskFrameBuffer(nullptr),
	mBloomBlurHorizontalFrameBuffer(nullptr),
	mBloomBlurVerticalFrameBuffer(nullptr),
	mBloomBlendFrameBuffer(nullptr),
	mIsRunning(true),
	hdr(false),
	bloom(false)
{
}

Game::~Game()
{
}

bool Game::Init()
{
	Random::Init();

	if (!mEngine.Init(WINDOW_WIDTH, WINDOW_HEIGHT, SUB_SAMPLES, VSYNC, IS_FULLSCREEN, MOUSE_CAPTURED, TITLE))
	{
		return false;
	}

	Renderer* renderer = mEngine.GetRenderer();

	mCamera = new Camera(mEngine.GetRenderer());
	mCamera->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	mLights.CreateBuffer(mEngine.GetRenderer());

	mShadowMap = new ShadowMap();

	PROFILE_SCOPE(LOAD_DATA);

	LoadShaders();

	int width = renderer->GetWidth();
	int height = renderer->GetHeight();

	// Create frame buffers
	mMainFrameBuffer = renderer->CreateMultiSampledFrameBuffer(width, height, renderer->GetNumSubsamples(), AssetManager::LoadShader("hdrGamma"));
	mBloomMaskFrameBuffer = renderer->CreateFrameBuffer(width / 2, height / 2, AssetManager::LoadShader("bloomMask"));
	mBloomBlurHorizontalFrameBuffer = renderer->CreateFrameBuffer(width / 4, height / 4, AssetManager::LoadShader("bloomBlurHorizontal"));
	mBloomBlurVerticalFrameBuffer = renderer->CreateFrameBuffer(width / 4, height / 4, AssetManager::LoadShader("bloomBlurVertical"));
	mBloomBlendFrameBuffer = renderer->CreateFrameBuffer(width, height, AssetManager::LoadShader("bloomBlend"));

	LoadGameData();

	return true;
}

void Game::Shutdown()
{
	UnloadGameData();

	delete mCamera;

	delete mShadowMap;

	delete mSkybox;

	mEngine.Shutdown();
}

void Game::LoadShaders() const
{
	AssetManager::LoadShader("color", "Shaders/color.vert", "Shaders/color.frag");
	AssetManager::LoadShader("phong", "Shaders/phong.vert", "Shaders/phong.frag");
	AssetManager::LoadShader("skinned", "Shaders/Animation/skinned.vert", "Shaders/phong.frag");
	AssetManager::LoadShader("texture", "Shaders/texture.vert", "Shaders/texture.frag");
	AssetManager::LoadShader("instance", "Shaders/instance.vert", "Shaders/phong.frag");
	AssetManager::LoadShader("shadowDepth", "Shaders/Shadow/shadowDepth.vert", "Shaders/Shadow/shadowDepth.frag");
	AssetManager::LoadShader("shadowDebug", "Shaders/screen.vert", "Shaders/Shadow/shadowDebug.frag");
	AssetManager::LoadShader("pointShadowDepth", "Shaders/Shadow/pointShadowDepth.vert", "Shaders/Shadow/pointShadowDepth.frag", "Shaders/Shadow/pointShadowDepth.geom");
	//AssetManager::LoadShader("invertedColor", "Shaders/screen.vert", "Shaders/Postprocess/invertedColor.frag");
	//AssetManager::LoadShader("grayScale", "Shaders/screen.vert", "Shaders/Postprocess/grayScale.frag");
	//AssetManager::LoadShader("sharpenKernel", "Shaders/screen.vert", "Shaders/Postprocess/sharpenKernel.frag");
	//AssetManager::LoadShader("blurKernel", "Shaders/screen.vert", "Shaders/Postprocess/blurKernel.frag");
	//AssetManager::LoadShader("edgeDetectKernel", "Shaders/screen.vert", "Shaders/Postprocess/edgeDetectKernel.frag");
	AssetManager::LoadShader("copyScreen", "Shaders/screen.vert", "Shaders/copyScreen.frag");
	AssetManager::LoadShader("bloomMask", "Shaders/screen.vert", "Shaders/Postprocess/Bloom/bloomMask.frag");
	AssetManager::LoadShader("bloomBlurHorizontal", "Shaders/screen.vert", "Shaders/Postprocess/Bloom/bloomBlurHorizontal.frag");
	AssetManager::LoadShader("bloomBlurVertical", "Shaders/screen.vert", "Shaders/Postprocess/Bloom/bloomBlurVertical.frag");
	AssetManager::LoadShader("bloomBlend", "Shaders/screen.vert", "Shaders/Postprocess/Bloom/bloomBlend.frag");
	Shader* hdrGammaShader = AssetManager::LoadShader("hdrGamma", "Shaders/screen.vert", "Shaders/hdrGamma.frag");
	hdrGammaShader->SetActive();
	hdrGammaShader->SetBool("hdr", hdr);
	hdrGammaShader->SetFloat("exposure", 1.0f);
	AssetManager::LoadShader("reflection", "Shaders/EnvironmentMapping/environmentMap.vert", "Shaders/EnvironmentMapping/reflection.frag");
	AssetManager::LoadShader("refraction", "Shaders/EnvironmentMapping/environmentMap.vert", "Shaders/EnvironmentMapping/refraction.frag");
	AssetManager::LoadShader("skybox", "Shaders/skybox.vert", "Shaders/skybox.frag");
}

void Game::LoadGameData()
{
	Texture* texture = AssetManager::LoadTexture("Assets/matrix.jpg", TextureType::Emission);
	Texture* texture3 = AssetManager::LoadTexture("Assets/container2.png", TextureType::Diffuse);
	Texture* texture4 = AssetManager::LoadTexture("Assets/container2_specular.png", TextureType::Specular);
	AssetManager::LoadTexture("Assets/lightSphere.png", TextureType::Diffuse);
	Texture* woodTexture = AssetManager::LoadTexture("Assets/wood.png", TextureType::Diffuse);
	Texture* wallTexture = AssetManager::LoadTexture("Assets/brickwall.jpg", TextureType::Diffuse);
	Texture* wallNormalTexture = AssetManager::LoadTexture("Assets/brickwall_normal.jpg", TextureType::Normal);

	Material* lightSphereMaterial = new Material();
	lightSphereMaterial->SetShader(AssetManager::LoadShader("texture"));
	lightSphereMaterial->AddTexture(AssetManager::LoadTexture("Assets/lightSphere.png"));
	AssetManager::Get()->SaveMaterial("lightSphere", lightSphereMaterial);

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
	mSkybox->SetShader(AssetManager::LoadShader("skybox"));
	CubeMap* sky = mSkybox->GetCubeMap();
	MaterialCubeMap* reflectiveMat = new MaterialCubeMap();
	reflectiveMat->SetCubeMap(sky);
	reflectiveMat->SetShader(AssetManager::LoadShader("reflection"));
	AssetManager::Get()->SaveMaterial("reflection", reflectiveMat);

	MaterialCubeMap* refractiveMat = new MaterialCubeMap();
	refractiveMat->SetCubeMap(sky);
	refractiveMat->SetShader(AssetManager::LoadShader("refraction"));
	AssetManager::Get()->SaveMaterial("refraction", refractiveMat);

	//Texture* rockTexture = AssetManager::LoadTexture("Assets/models/rock/rock.png");
	//rockTexture->SetType(TextureType::Diffuse);
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


	float time = 0.1f;
	for (size_t i = 0; i < 10; ++i)
	{
		Entity3D* vampire = new Entity3D("Assets/models/vampire/dancing_vampire.dae");
		vampire->SetScale(0.05f);
		vampire->SetPosition(vampirePositions[i]);
		AddGameEntity(vampire);

		vampires.emplace_back(vampire);
	}

	Entity3D* sponza = new Entity3D("Assets/models/Sponza/sponza.obj");
	sponza->SetPosition(glm::vec3(0.0f, -5.0, 0.0f));
	sponza->SetScale(0.125);
	sponza->FaceDirection(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//sponza->SetYaw(-90.0f);
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
	squidward->FaceDirection(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	AddGameEntity(squidward);

	Entity3D* squidward2 = new Entity3D("Assets/models/SquidwardDance/Rumba Dancing.dae");
	squidward2->SetPosition(glm::vec3(10.0f, -5.0f, -15.0f));
	squidward2->SetScale(0.35f);
	AddGameEntity(squidward2);


	//squidward->SetMaterialShader("tt", refractiveShader);
	Material* m = squidward->GetModel()->GetMaterial("ttmat");
	//m->AddTexture(texture);
	m->SetSpecularIntensity(0.0f);

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

	Entity3D* fortune2 = new Entity3D("Assets/models/MissFortune/MissFortune.dae");
	fortune2->SetPosition(glm::vec3(-5.0f, -5.0f, -25.0f));
	fortune2->SetScale(0.25f);
	AddGameEntity(fortune2);

	Entity3D* fortune = new Entity3D("Assets/models/MissFortune2/MissFortune2.dae");
	fortune->SetPosition(glm::vec3(5.0f, -5.0f, -25.0f));
	fortune->SetScale(0.25f);
	AddGameEntity(fortune);

	//glm::vec3 lightPosition(1.0f, 10.0f, 3.0f);
	glm::vec3 lightPosition = lightDir * -dist;
	mShadowMap->SetShader(AssetManager::LoadShader("shadowDepth"));
	pos = lightPosition;

	DirectionalLight* dirLight = mLights.AllocateDirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(-0.2f, -1.0f, -0.3f));
	dirLight->data.usesShadow = true;

	PointLight* pointLight = mLights.AllocatePointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 3.0f, 60.0f), 1.0f, 0.014f, 0.0007f);
	pointLight->data.diffuseIntensity = 70.0f;
	pointLight->data.specularIntensity = 50.0f;
	Sphere* lightSphere = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSphere->SetMaterial(lightSphereMaterial);
	lightSphere->SetPosition(glm::vec3(1.0f, 3.0f, 60.0f));
	AddGameEntity(lightSphere);

	//SpotLight* spotLight = mLights.AllocateSpotLight(glm::vec4(0.25f, 0.61f, 1.0f, 1.0f), glm::vec3(-0.7f, 3.0, 0.0f), glm::vec3(0.0, -1.0f, 0.0f),
	//	glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(16.0f)), 1.0f, 0.09f, 0.032f);
	//Sphere* lightSphere2 = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//lightSphere2->SetMaterial(reflectiveMat);
	//lightSphere2->SetPosition(glm::vec3(-0.7f, 3.0, 0.0f));
	//AddGameEntity(lightSphere2);

	PointLight* pointLight2 = mLights.AllocatePointLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 3.0f, -120.0f), 1.0f, 0.014f, 0.0007f);
	pointLight2->data.diffuseIntensity = 70.0f;
	pointLight2->data.specularIntensity = 900.0f;
	Sphere* lightSphere3 = new Sphere(0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	lightSphere3->SetMaterial(lightSphereMaterial);
	lightSphere3->SetPosition(glm::vec3(0.0f, 3.0f, -120.0f));
	AddGameEntity(lightSphere3);

	// Since all ShaderProgram objects are attached to a Shader object, it's safe to de-allocate them here
	AssetManager::Get()->ClearShaderPrograms();
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
	Mouse* mouse = mEngine.GetMouse();
	Keyboard* keyboard = mEngine.GetKeyboard();

	mouse->ResetState();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		mEngine.GetEngineUI()->ProcessSDLEvent(event);
		switch (event.type)
		{
		case SDL_QUIT:
			// Check to see when user quits the game
			mIsRunning = false;
			break;
		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				ResizeWindow(event);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			// Mouse click down
			mouse->SetButtonDown(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			// Mouse click release
			mouse->SetButtonUp(event.button.button);
			break;
		case SDL_MOUSEWHEEL:
			// Mouse wheel scroll
			mouse->SetScrollDir(event.wheel.y);
			break;
		}
	}

	ProcessMouseInput(mouse);

	const Uint8* keyboardState = keyboard->GetState();

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

	// Toggle camera modes
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_V))
	{
		mCamera->ToggleCameraModes();
	}

	// Toggle the main directional light
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_SPACE))
	{
		bool light = mLights.GetLights().directionalLight[0].data.isEnabled;
		mLights.GetLights().directionalLight[0].data.isEnabled = !light;
	}

	// Capture mouse
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_M))
	{
		mouse->ToggleMouseCapture();
	}

	// HDR/Exposure
	Shader* shader = AssetManager::LoadShader("hdrGamma");
	Shader* bloomAdd = AssetManager::LoadShader("bloomBlend");

	// Toggle hdr
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_H))
	{
		hdr = !hdr;
		shader->SetActive();
		shader->SetBool("hdr", hdr);
	}
	// Toggle bloom
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_B))
	{
		bloom = !bloom;

		bloomAdd->SetActive();
		bloomAdd->SetBool("bloom", bloom);
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
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_UP))
	{
		size += 10.0f;
	}
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_DOWN))
	{
		size -= 10.0f;
	}
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_LEFT))
	{
		far -= 10.0f;
	}
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_RIGHT))
	{
		far += 10.0f;
	}
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_L))
	{
		dist -= 10.0f;
		pos = (lightDir * -dist);
	}
	if (keyboard->HasLeadingEdge(keyboardState, SDL_SCANCODE_K))
	{
		dist += 10.0f;
		pos = (lightDir * -dist);
	}

	if (keyboardState[SDL_SCANCODE_KP_0])
	{
		vampires[0]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_1])
	{
		vampires[1]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_2])
	{
		vampires[2]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_3])
	{
		vampires[3]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_4])
	{
		vampires[4]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_5])
	{
		vampires[5]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_6])
	{
		vampires[6]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_7])
	{
		vampires[7]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_8])
	{
		vampires[8]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}
	if (keyboardState[SDL_SCANCODE_KP_9])
	{
		vampires[9]->GetComponent<AnimationComponent>()->SetTime(0.0f);
	}

	// Save previous key inputs
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_V);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_SPACE);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_H);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_B);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_UP);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_DOWN);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_LEFT);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_RIGHT);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_L);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_K);
	keyboard->SavePrevKeyState(keyboardState, SDL_SCANCODE_M);

	for (auto e : mEntities)
	{
		e->ProcessInput(keyboardState, keyboard, mouse);
	}

	mEngine.GetConsole()->ProcessInput(keyboardState, keyboard, mouse);
}

void Game::ProcessMouseInput(Mouse* mouse)
{
	Uint8 buttonDown = mouse->GetButtonDown();
	Uint8 buttonUp = mouse->GetButtonUp();
	Sint32 scroll = mouse->GetScrollDir();
	//// Single left click
	//if (buttonDown == SDL_BUTTON_LEFT)
	//{
	//	std::cout << "Left Click\n";
	//}
	//// Single right click
	//if (buttonDown == SDL_BUTTON_RIGHT)
	//{
	//	std::cout << "Right Click\n";
	//}
	//// Single scroll wheel click (mouse 3 click)
	//if (buttonDown == SDL_BUTTON_MIDDLE)
	//{
	//	std::cout << "Scroll Click\n";
	//}
	//// Single mouse 4 click
	//if (buttonDown == SDL_BUTTON_X1)
	//{
	//	std::cout << "Mouse 4 Click\n";
	//}
	//// Single mouse 5 click
	//if (buttonDown == SDL_BUTTON_X2)
	//{
	//	std::cout << "Mouse 5 Click\n";
	//}

	//Uint8 mouse_state = SDL_GetMouseState(NULL, NULL);
	//// Left click hold
	//if (mouse_state & SDL_BUTTON_LMASK)
	//{
	//	std::cout << "Left Click hold\n";
	//}
	//// Right click hold
	//if (mouse_state & SDL_BUTTON_RMASK)
	//{
	//	std::cout << "Right Click hold\n";
	//}
	//// Scroll click hold
	//if (mouse_state & SDL_BUTTON_MMASK)
	//{
	//	std::cout << "Scroll Click hold\n";
	//}
	//// Mouse 4 click hold
	//if (mouse_state & SDL_BUTTON_X1MASK)
	//{
	//	std::cout << "Mouse 4 Click hold\n";
	//}
	//// Mouse 5 click hold
	//if (mouse_state & SDL_BUTTON_X2MASK)
	//{
	//	std::cout << "Mouse 5 Click hold\n";
	//}

	//// Left click release
	//if (buttonUp == SDL_BUTTON_LEFT)
	//{
	//	std::cout << "Left Click Release\n";
	//}
	//// Right click release
	//if (buttonUp == SDL_BUTTON_RIGHT)
	//{
	//	std::cout << "Right Click Release\n";
	//}
	//// Scroll click release
	//if (buttonUp == SDL_BUTTON_MIDDLE)
	//{
	//	std::cout << "Scroll Click Release\n";
	//}
	//// Mouse 4 click release
	//if (buttonUp == SDL_BUTTON_X1)
	//{
	//	std::cout << "Mouse 4 Click Release\n";
	//}
	//// Mouse 5 click release
	//if (buttonUp == SDL_BUTTON_X2)
	//{
	//	std::cout << "Mouse 5 Click Release\n";
	//}

	//// Scroll wheel up
	//if (scroll >= 1)
	//{
	//	std::cout << "Scroll Up\n";
	//}
	//// Scroll wheel down
	//if (scroll <= -1)
	//{
	//	std::cout << "Scroll Down\n";
	//}

	if (mouse->MouseCaptured() == SDL_TRUE)
	{
		mouse->CalculateMovement();
	}
}

void Game::Update(float deltaTime)
{
	PROFILE_SCOPE(UPDATE);

	for (auto e : mEntities)
	{
		e->Update(deltaTime);
	}

	mCamera->Update(deltaTime, mEngine.GetMouse());

	PROFILE_SCOPE(WAIT_JOBS);
	JobManager::Get()->WaitForJobs();
}

void Game::Render()
{
	PROFILE_SCOPE(RENDER);

	mEngine.GetEngineUI()->SetUI();

	mCamera->SetBuffer();

	mLights.SetBuffer();

	Renderer* renderer = mEngine.GetRenderer();

	renderer->ClearBuffers();

	{
		PROFILE_SCOPE(RENDER_SHADOW_MAP);

		//std::cout << size << " " << near << " " << far << " " << pos.x << " " << pos.y << " " << pos.z << "\n";

		// Render to shadow map
		mShadowMap->SetActive(size, near, far, pos, glm::vec3(0.0f, 0.0f, 0.0f));
		RenderScene(mShadowMap->GetShader());

		// End shadow render pass
		mShadowMap->End(renderer->GetWidth(), renderer->GetHeight());
		mShadowMap->BindShadowMapToShader(AssetManager::LoadShader("phong"), "textureSamplers.shadow");
		mShadowMap->BindShadowMapToShader(AssetManager::LoadShader("skinned"), "textureSamplers.shadow");
	}

	// Draw to main multisampled frame buffer
	mMainFrameBuffer->SetActive();

	// Render scene as normal
	RenderScene();
	
	mMainFrameBuffer->BlitBuffers();
	// Use the multisampled texture and mask off dark spots
	mBloomMaskFrameBuffer->SetActive();
	mBloomMaskFrameBuffer->Draw(mMainFrameBuffer->GetTexture());
	// Use the masked off texture and blur horizontally and readjust shader width
	mBloomBlurHorizontalFrameBuffer->SetActive();
	mBloomBlurHorizontalFrameBuffer->GetShader()->SetFloat("width", static_cast<float>(renderer->GetWidth() / 4));
	mBloomBlurHorizontalFrameBuffer->Draw(mBloomMaskFrameBuffer->GetTexture());
	// Use the horizontally blurred texture to blur vertically to create the blurred image and readjust shader height
	mBloomBlurVerticalFrameBuffer->SetActive();
	mBloomBlurVerticalFrameBuffer->GetShader()->SetFloat("height", static_cast<float>(renderer->GetHeight() / 4));
	mBloomBlurVerticalFrameBuffer->Draw(mBloomBlurHorizontalFrameBuffer->GetTexture());
	// Use the multisampled texture and the blurred texture to additively blend them
	mBloomBlendFrameBuffer->SetActive();
	renderer->CreateBlend(mBloomBlendFrameBuffer->GetShader(), mMainFrameBuffer->GetTexture(), mBloomBlurVerticalFrameBuffer->GetTexture(), static_cast<int>(TextureType::FrameBuffer));
	mBloomBlendFrameBuffer->Draw(mMainFrameBuffer->GetTexture());
	// Draw the final image
	renderer->SetDefaultFrameBuffer();
	// Set the main frame buffer's shader active here (since SetDefaultFrameBuffer() does not activate a shader) and add HDR/gamma correction
	mMainFrameBuffer->GetShader()->SetActive();
	mMainFrameBuffer->Draw(mBloomBlendFrameBuffer->GetTexture());


	mShadowMap->DrawDebug(AssetManager::LoadShader("shadowDebug"));
	glViewport(0, 0, renderer->GetWidth(), renderer->GetHeight());

	mEngine.GetEngineUI()->Render();

	renderer->EndFrame();
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

void Game::ResizeWindow(const SDL_Event& event)
{
	SDL_Window* window = SDL_GetWindowFromID(event.window.windowID);
	if (window)
	{
		int width = event.window.data1;
		int height = event.window.data2;

		mEngine.GetRenderer()->Resize(width, height);

		float ratio = static_cast<float>(width) / static_cast<float>(height);

		// Set new camera aspect ratio
		mCamera->SetAspectRatio(ratio);
	}
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
