#include "Plane.h"
#include <iostream>
#include "../Graphics/VertexLayouts.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/Shader.h"
#include "../Graphics/Material.h"

Plane::Plane() :
	Entity3D(),
	mVertexBuffer(nullptr),
	mMaterial(nullptr)
{
	VertexSimple vertices[] = {
		glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f),   // top right
		glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f),  // bottom right
		glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), // bottom left
		glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f)   // top left
	};

	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	mVertexBuffer = new VertexBuffer(vertices, indices, sizeof(vertices), sizeof(indices), sizeof(vertices) / sizeof(VertexSimple), sizeof(indices) / sizeof(unsigned int), VertexLayout::VertexSimple);
}

Plane::~Plane()
{
	std::cout << "Delete plane" << std::endl;
	delete mVertexBuffer;
}

void Plane::OnUpdate(float deltaTime)
{
	Entity3D::OnUpdate(deltaTime);
}

void Plane::OnDraw()
{
	mMaterial->SetActive();
	mMaterial->GetShader()->SetMat4("model", mModelMatrix);

	glDisable(GL_CULL_FACE);

	mVertexBuffer->Draw();

	glEnable(GL_CULL_FACE);
}

void Plane::OnDraw(Shader* s)
{
	mMaterial->SetActive();
	s->SetActive();

	s->SetMat4("model", mModelMatrix);

	mVertexBuffer->Draw();
}
