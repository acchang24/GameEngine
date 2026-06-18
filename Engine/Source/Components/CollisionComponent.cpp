#include "CollisionComponent.h"
#include <iostream>
#include "../Physics/Physics.h"

CollisionComponent::CollisionComponent(Entity* owner, Physics* physics, CollisionShapeType shapeType, BodyType bodyType) :
	Component(owner),
	mPhysics(physics),
	mShapeType(shapeType),
	mBodyType(bodyType)
{
	mPhysics->AddCollider(this);
}

CollisionComponent::~CollisionComponent()
{
	std::cout << "Deleted CollisionComponent\n";

	mPhysics->RemoveCollider(this);
}

void CollisionComponent::Update(float deltaTime, const EngineContext& engineContext)
{
}

AABBComponent2D::AABBComponent2D(Entity* owner, Physics* physics, BodyType bodyType) :
	CollisionComponent(owner, physics, CollisionShapeType::AABB2D, bodyType),
	mOwner2D(owner),
	mBox()
{
}

AABBComponent2D::~AABBComponent2D()
{
	std::cout << "Deleted AABBComponent2D\n";
}

void AABBComponent2D::Update(float deltaTime, const EngineContext& engineContext)
{
	// Update the box location
	mBox.min = GetMin();
	mBox.max = GetMax();
}


const glm::vec2 AABBComponent2D::GetMin() const
{
	glm::vec2 min = mOwner2D->GetPosition2D();
	const glm::vec2& scale = mOwner2D->GetScale2D();

	// Minimum x value
	min.x -= (mBox.width * scale.x * 0.5f);
	// Minimum y value
	min.y -= (mBox.height * scale.y * 0.5f);

	return min;
}

const glm::vec2 AABBComponent2D::GetMax() const
{
	glm::vec2 max = mOwner2D->GetPosition2D();
	const glm::vec2& scale = mOwner2D->GetScale2D();

	// Maximum x value
	max.x += (mBox.width * scale.x * 0.5f);
	// Maximum y value
	max.y += (mBox.height * scale.y * 0.5f);

	return max;
}


CircleComponent::CircleComponent(Entity* owner, Physics* physics, float radius, BodyType bodyType) :
	CollisionComponent(owner, physics, CollisionShapeType::Circle, bodyType),
	mOwner2D(owner),
	mCircle(radius)
{
}

CircleComponent::~CircleComponent()
{
	std::cout << "Deleted CircleComponent\n";
}

OBBComponent2D::OBBComponent2D(Entity* owner, Physics* physics, BodyType bodyType) :
	CollisionComponent(owner, physics, CollisionShapeType::OBB2D, bodyType),
	mOwner2D(owner),
	mOBB()
{
}

OBBComponent2D::~OBBComponent2D()
{
	std::cout << "Deleted OOBBComponent2D\n";
}

std::array<glm::vec2, 4> OBBComponent2D::GetCorners() const
{
	float rotation = mOwner2D->GetRotation2D();

	// Get the box's local x axis (positive width direction)
	glm::vec2 xAxis = glm::vec2(cos(rotation), sin(rotation));
	// Get the box's local y axis (positive height direction)
	glm::vec2 yAxis = glm::vec2(-sin(rotation), cos(rotation));

	const glm::vec2& halfExtents = GetHalfExtents();
	// Scale axis by half of box width
	glm::vec2 hx = xAxis * halfExtents.x;
	// Scale axis by half of box height
	glm::vec2 hy = yAxis * halfExtents.y;

	const glm::vec2& center = mOwner2D->GetPosition2D();
	std::array<glm::vec2, 4> corners = {
		center - hx - hy, // Bottom-left
		center + hx - hy, // Bottom-right
		center + hx + hy, // Top-right
		center - hx + hy  // Top-left
	};

	return corners;
}
