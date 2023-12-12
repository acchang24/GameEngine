#include <iostream>
#include <unordered_map>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define WIDTH 1280
#define HEIGHT 720

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

std::unordered_map<int, bool> test;

int main()
{
	test[GLFW_KEY_ESCAPE] = false;
	test[GLFW_KEY_SPACE] = false;

	// Initialize GLFW
	glfwInit();
	// Set major/minor contexts to 4
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	// Use core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFW window
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Game", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Make the newly created window the main context on the current thread
	glfwMakeContextCurrent(window);

	// Initialize GLAD before OpenGl function calls
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Create viewport
	// Sets the location of lower left corner (0, 0)
	// Sets width/height of rendering window to the size of GLFW window size
	glViewport(0, 0, WIDTH, HEIGHT);

	// Tell GLFW to call window resize function on every window resize
	// This registers the callback function
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallBack);

	// Enable v-sync
	glfwSwapInterval(1);

	// Render loop
	// glfwWindowShouldClose() checks at the start of each loop iteration to see if GLFW needs to be closed
	while (!glfwWindowShouldClose(window))
	{
		// Specify color to clear the screen
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		// Clear the color buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Check to see if any events are triggered (inputs) and updates the window state
		glfwPollEvents();

		ProcessInput(window);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	// Clean/delete all GLFW resources
	glfwTerminate();

	return 0;
}

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !test[GLFW_KEY_SPACE])
	{
		test[GLFW_KEY_SPACE] = true;
		std::cout << "space" << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && test[GLFW_KEY_SPACE])
	{
		test[GLFW_KEY_SPACE] = false;
	}
}