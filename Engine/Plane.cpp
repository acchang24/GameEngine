#include "Plane.h"
#include <iostream>
#include "VertexLayouts.h"
#include "VertexBuffer.h"

Plane::Plane() :
	Entity3D()
{
	VertexTexture vertices[] = {
		glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f),   // top right
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f),  // bottom right
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), // bottom left
		glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)   // top left
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	mVertexBuffer = new VertexBuffer(vertices, indices, sizeof(vertices), sizeof(indices), sizeof(vertices) / sizeof(VertexTexture), sizeof(indices) / sizeof(unsigned int), VertexLayout::VertexTexture);
}

Plane::~Plane()
{
	std::cout << "Delete plane" << std::endl;
}

void Plane::OnUpdate(float deltaTime)
{
	Entity3D::OnUpdate(deltaTime);
}

void Plane::OnDraw()
{

}
