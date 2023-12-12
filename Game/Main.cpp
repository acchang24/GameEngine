#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define WIDTH 1280
#define HEIGHT 720

void FrameBufferSizeCallBack(GLFWwindow* window, int width, int height);

int main()
{
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
		// Check to see if any events are triggered (inputs) and updates the window state
		glfwPollEvents();

		// Swap the color buffer
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