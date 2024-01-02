#pragma once
#include <unordered_map>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LightConstants.h"

class Shader;
class Texture;
class Entity;
class Camera;
class AssetManager;
class FrameBuffer;
class Skybox;

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

	void ProcessMouseInput(GLFWwindow* window);

	// Resizes and adjusts the viewport as well as updates the perspective matrix
	// when the user changes the window size. Registers on every window resize.
	// @param - GLFWwindow* for the window being resized
	// @param - int for the window's width
	// @param - int for the window's height
	static void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);

	void AddGameEntity(Entity* e) { mEntities.emplace_back(e); }

	// Allocator for point lights
	// @param - const glm::vec4& for the point light's color
	// @param - const glm::vec3& for the light's position
	// @param - float for attenuation constant term
	// @param - float for attenuation linear term
	// @param - float for attenuation quadratic term
	// @return - PointLight* for a newly allocated PointLight
	PointLight* AllocatePointLight(const glm::vec4& color, const glm::vec3& position, float constant, float linear, float quadratic);

	// Allocator for directional lights
	// @param - const glm::vec4& for the light's color
	// @param - const glm::vec3& for the light's direction
	// @return - DirectionalLight* a newly allocated DirectionalLight
	DirectionalLight* AllocateDirectionalLight(const glm::vec4& color, const glm::vec3& direction);

	// Allocator for spot lights
	// @param - const glm::vec4& for the light's color
	// @param - const glm::vec3& for the light's position
	// @param - const glm::vec3& for the light's direction
	// @param - float for the spotlight's inner circle cutoff
	// @param - float for the spotlight's outer circle cutoff
	// @param - float for attenuation constant term
	// @param - float for attenuation linear term
	// @param - float for attenuation quadratic term
	// @return - SpotLight* for a newly allocated SpotLight
	SpotLight* AllocateSpotLight(const glm::vec4& color, const glm::vec3& pos, const glm::vec3& dir, float cutoff, float outerCutoff, float constant, float linear, float quadratic);

	// Loops through each light and deletes them
	void DeAllocateLights();

private:
	// std::unordered_map of keyboard inputs on the previous frame/loop.
	// Takes key press as an int and stores the key's state as a bool.
	std::unordered_map<int, bool> mPrevInputs;

	// std::vector of game entities
	std::vector<Entity*> mEntities;

	// Pointer to a GLFWwindow, serves as the main game window.
	GLFWwindow* mWindow;

	// The game's camera
	Camera* mCamera;

	// Frame buffer
	FrameBuffer* mFrameBuffer;

	// Skybox
	Skybox* mSkybox;

	// Array of different lights
	LightArrays mLightArrays;

	unsigned int uboCamera;

	double mMousePosX;
	double mMousePosY;

	double mMousePrevX;
	double mMousePrevY;

	bool mFirstMouse;

	// Bool to check if the game is running.
	bool mIsRunning;
};
