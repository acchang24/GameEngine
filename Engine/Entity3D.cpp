#include "Entity3D.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBuffer.h"
#include "Shader.h"
#include "Material.h"

Entity3D::Entity3D() :
	Entity(),
	mVertexBuffer(nullptr),
	mMaterial(nullptr),
	mModel(glm::mat4(1.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mYaw(0.0f),
	mPitch(0.0f),
	mRoll(0.0f)
{
}

Entity3D::~Entity3D()
{
	std::cout << "Delete entity 3D" << std::endl;
	delete mVertexBuffer;
	delete mMaterial;
}

void Entity3D::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	OnUpdate(deltaTime);
}

void Entity3D::Draw()
{
	OnDraw();
}

void Entity3D::OnUpdate(float deltaTime)
{
	mModel = glm::mat4(1.0f);

	// Translate
	mModel = glm::translate(mModel, mPosition);

	// Rotate
	mModel = glm::rotate(mModel, glm::radians(mRoll), glm::vec3(0.0f, 0.0f, 1.0f));
	mModel = glm::rotate(mModel, glm::radians(mPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	mModel = glm::rotate(mModel, glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	// Scale
	mModel = glm::scale(mModel, mScale);
}

void Entity3D::OnDraw()
{
	mMaterial->SetActive();
	mMaterial->GetShader()->SetMat4("model", mModel);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	mVertexBuffer->Draw();
}
