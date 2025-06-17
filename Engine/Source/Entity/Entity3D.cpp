#include "Entity3D.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Components/AnimationComponent.h"
#include "../Graphics/Model.h"
#include "../Graphics/Shader.h"
#include "../MemoryManager/AssetManager.h"

Entity3D::Entity3D() :
	Entity(),
	mUpdateModelMatrixJob(this),
	mModelMatrix(glm::mat4(1.0f)),
	mQuatRotation(glm::quat()),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mModel(nullptr),
	mInstanceBuffer(0)
{
}

Entity3D::Entity3D(const std::string& fileName):
	Entity(),
	mUpdateModelMatrixJob(this),
	mModelMatrix(glm::mat4(1.0f)),
	mQuatRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(glm::vec3(1.0f, 1.0f, 1.0f)),
	mModel(nullptr),
	mInstanceBuffer(0)
{
	Model* model = AssetManager::LoadModel(fileName);

	if (model)
	{
		// Use the cached model
		mModel = model;

		// Check to see if there are animations
		if (model->HasAnimations())
		{
			// Check to see the skeleton is already loaded
			Skeleton* skeleton = AssetManager::LoadSkeleton(fileName);

			if (skeleton)
			{
				// Create an animation component for this entity using the cached skeleton
				AnimationComponent* animComp = new AnimationComponent(this, skeleton);
			}
			else
			{
				std::cout << "Model loading error:: duplicated model file: " << fileName << " does not have a matching skeleton loaded.\n";
			}
		}
	}
	else
	{
		// Create a new model
		mModel = new Model(fileName, this);
	}
}

Entity3D::~Entity3D()
{
	std::cout << "Deleted entity 3D\n";

	glDeleteBuffers(1, &mInstanceBuffer);
}

void Entity3D::MakeInstance(unsigned int numInstances, const void* data)
{
	glGenBuffers(1, &mInstanceBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mInstanceBuffer);
	glBufferData(GL_ARRAY_BUFFER, numInstances * sizeof(glm::mat4), data, GL_STATIC_DRAW);

	mModel->MakeInstance(numInstances);
}

void Entity3D::ProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse)
{
	Entity::ProcessInput(keyState, keyboard, mouse);

	OnProcessInput(keyState, keyboard, mouse);
}

void Entity3D::Update(float deltaTime)
{
	Entity::Update(deltaTime);

	if (mInstanceBuffer == 0)
	{
		OnUpdate(deltaTime);
	}
}

void Entity3D::Draw()
{
	OnDraw();
}

void Entity3D::Draw(Shader* shader)
{
	if (mInstanceBuffer == 0)
	{
		OnDraw(shader);
	}
}

void Entity3D::OnProcessInput(const Uint8* keyState, Keyboard* keyboard, const Mouse* mouse)
{
}

void Entity3D::OnUpdate(float deltaTime)
{
	// Update model matrix on seprate thread		
	JobManager::Get()->AddJob(&mUpdateModelMatrixJob);
	
	//CalculateWorldTransform();
}

void Entity3D::OnDraw()
{
	if (mModel->HasAnimations())
	{
		GetComponent<AnimationComponent>()->UpdateSkeletonBuffer();
	}

	mModel->Draw(mModelMatrix);
}

void Entity3D::OnDraw(Shader* shader)
{
	if (mModel->HasAnimations())
	{
		GetComponent<AnimationComponent>()->UpdateSkeletonBuffer();
	}

	shader->SetActive();
	shader->SetBool("isSkinned", mModel->HasAnimations());

	mModel->Draw(shader, mModelMatrix);
}

void Entity3D::CalculateWorldTransform()
{
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 translate = glm::translate(model, mPosition);

	glm::mat4 rotation = glm::toMat4(mQuatRotation);

	glm::mat4 scale = glm::scale(model, mScale);

	mModelMatrix = translate * rotation * scale;
}

void Entity3D::RotateFromInput(float deltaYawDeg, float deltaPitchDeg, float deltaRollDeg)
{
	// Convert degrees to radians
	float deltaYawRad = glm::radians(deltaYawDeg);
	float deltaPitchRad = glm::radians(deltaPitchDeg);
	float deltaRollRad = glm::radians(deltaRollDeg);

	// Create incremental quaternions
	glm::quat qYaw = glm::angleAxis(deltaYawRad, glm::vec3(0.0f, 1.0f, 0.0f)); // Y axis
	glm::quat qPitch = glm::angleAxis(deltaPitchRad, glm::vec3(1.0f, 0.0f, 0.0f)); // X axis
	glm::quat qRoll = glm::angleAxis(deltaRollRad, glm::vec3(0.0f, 0.0f, 1.0f)); // Z axis

	// Apply order: yaw (global Y) -> pitch (local X) -> roll (local Z)
	mQuatRotation = qYaw * mQuatRotation;
	mQuatRotation = mQuatRotation * qPitch;
	mQuatRotation = mQuatRotation * qRoll;

	mQuatRotation = glm::normalize(mQuatRotation);
}

void Entity3D::FaceDirection(const glm::vec3& forwardDir, const glm::vec3& up)
{
	// Make sure forward dir is normalized
	glm::vec3 forward = glm::normalize(forwardDir);
	// Get the right vector
	glm::vec3 right = glm::normalize(glm::cross(up, forward));
	// Update up bector
	glm::vec3 u = glm::cross(forward, right);
	// Right, Up, Forward
	glm::mat3 rotationMatrix(right, u, forward);
	// Create quaternion orientation
	mQuatRotation = glm::quat_cast(rotationMatrix);
}

void Entity3D::SetRotationFromEulerDegrees(const glm::vec3& eulerDegrees)
{
	// convert to radians
	glm::vec3 radians = glm::radians(eulerDegrees);

	// Create the quaternion from radian degrees
	mQuatRotation = glm::quat(radians);
}

void Entity3D::UpdateModelMatrixJob::DoJob()
{
	mEntity->CalculateWorldTransform();
}
