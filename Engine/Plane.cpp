#include "Plane.h"
#include <iostream>
#include "VertexLayouts.h"
#include "VertexBuffer.h"

Plane::Plane() :
	Entity3D()
{
	VertexTexture vertices[] = {
		glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f), // Top right
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), // Bottom right
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), // Bottom left
		glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f) // Top left
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Create a vertex buffer with the vertices
	mVertexBuffer = new VertexBuffer(vertices, indices, sizeof(vertices), sizeof(indices), sizeof(vertices) / sizeof(VertexTexture), sizeof(indices) / sizeof(unsigned int), VertexLayout::VertexTexture);
}

Plane::~Plane()
{
	std::cout << "Delete plane" << std::endl;
}

void Plane::OnUpdate(float deltaTime)
{

}

void Plane::OnDraw()
{

}
