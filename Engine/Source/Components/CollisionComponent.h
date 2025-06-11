#pragma once
#include "Component.h"
#include <array>
#include <functional>
#include <glm/glm.hpp>
#include "../Entity/Entity2D.h"

// Enum class for collision shape type
enum class CollisionShapeType
{
	AABB2D,
	Circle,
	OBB2D,
	Ray2D,
	AABB3D,
	Sphere,
	Capsule,
	Ray3D,
	Plane3D
};

// Enum class for the body type of the collision (how it interacts with other collisions)
enum class BodyType
{
	Static,
	Dynamic,
	Intersect
};

// Enum class for which side of the object a box collides with
enum class CollisionSide
{
	None,
	Top,
	Bottom,
	Left,
	Right
};

struct CollisionResult
{
	CollisionSide sideA;
	CollisionSide sideB;
};

// Struct for a 2D axis-aligned bounding box
struct AABB_2D
{
	AABB_2D() :
		min(glm::vec2(0.0f, 0.0f)),
		max(glm::vec2(0.0f, 0.0f)),
		width(0.0f),
		height(0.0f)
	{
	}

	glm::vec2 min;	// min x and y values
	glm::vec2 max;	// max x and y values
	float width;	// width of the entity
	float height;	// height of the entity
};

// Struct for a 2D circle
struct Circle2D
{
	Circle2D(float r) : 
		radius(r)
	{
	}

	float radius;	// Radius of the circle
};

// Struct for a 2D OBB
struct OBB_2D
{
	OBB_2D() :
		center(glm::vec2(0.0f, 0.0f)),
		halfExtents(glm::vec2(0.0f, 0.0f)),
		rotation(0.0f)
	{
	}

	// Gets the corners of the OBB
	// @return - std::array<glm::vec2, 4> for the corners
	std::array<glm::vec2, 4> GetCorners() const;

	glm::vec2 center;		// Center of the box
	glm::vec2 halfExtents;	// Half of the width and height of box
	float rotation;			// Rotation of owner in radians
};

class Physics;
struct CollisionResult;

class CollisionComponent : public Component
{
public:
	using CollisionCallback = std::function<void(Entity2D* other, const CollisionResult& result)>;

	// CollisionComponent constructor: adds this collision component to physics
	// @param - Entity* for the component's owner
	// @param - Physics* to add the collider to
	// @param - CollisionShapeType for the collision shape
	// @param - BodyType for the body type (static, dynamic)
	CollisionComponent(Entity* owner, Physics* physics, CollisionShapeType shapeType, BodyType bodyType);
	virtual ~CollisionComponent();

	// Override of Component::Update() for collision. 
	// This is a virtual function (overridable)
	virtual void Update(float deltaTime) override;

	// Gets the collision component's shape type
	// @return - CollisionShapeType for the shape type
	CollisionShapeType GetShapeType() const { return mShapeType; }

	// Gets the collision component's body type
	// @return - BodyType for the body type
	BodyType GetBodyType() const { return mBodyType; }

	// Setter for collision callback
	void SetOnCollision(const CollisionCallback& callback) { mOnCollision = callback; }

	// Call this during collision
	void OnCollision(Entity2D* other, const CollisionResult& result)
	{
		if (mOnCollision)
		{
			mOnCollision(other, result);
		}
	}

protected:
	// Pointer to physics system
	Physics* mPhysics;

	// Type of the shape used for collision
	CollisionShapeType mShapeType;

	// Body type of the shape
	BodyType mBodyType;

	// Callback function to do something after collision
	CollisionCallback mOnCollision;
};

class AABBComponent2D : public CollisionComponent
{
public:
	// AABBComponent2D constructor:
	// @param - Entity2D* for the owner
	// @param - Physics* for the physics system
	// @param - BodyType (optional, defaults to dynamic)
	AABBComponent2D(Entity2D* owner, Physics* physics, BodyType bodyType = BodyType::Dynamic);
	~AABBComponent2D();

	// Override Update for collision: 
	// Updates the collision box location based on owner's position
	// @param - float for delta time
	void Update(float deltaTime) override;

	// Gets the min x and y values of the box where an object can intersect
	// @return - const glm::vec2 for the min
	const glm::vec2 GetMin() const;

	// Gets the max x and y values of the box wherer an object can intersect
	// @return - const glm::vec2 for the max
	const glm::vec2 GetMax() const;

	// Gets the Entity2D owner of this collision component
	// @return - Entity2D* for the owner
	Entity2D* GetOwner() { return mOwner2D; }

	// Gets the the entity's collision box (AABB)
	// @return - const AABB_2D& for the axis aligned bounding box
	const AABB_2D& GetBox() const { return mBox; }

	// Gets the center of the AABB box
	// @return - const glm::vec2& for the center position
	const glm::vec2& GetCenter() const { return mOwner2D->GetPosition(); }

	// Sets the AABB's width and height
	// @param - const glm::vec2& for the size
	void SetBoxSize(const glm::vec2& size)
	{
		mBox.width = size.x;
		mBox.height = size.y;
	}

private:
	// Pointer to entity 2D
	Entity2D* mOwner2D;

	// Collision box
	AABB_2D mBox;
};


class CircleComponent : public CollisionComponent
{
public:
	// Circle constructor:
	// @param - Entity2D* for the owner
	// @param - Physics* for the physics system
	// @param - float for the radius
	// @param - BodyType (optional, defaults to dynamic)
	CircleComponent(Entity2D* owner, Physics* physics, float radius, BodyType bodyType = BodyType::Dynamic);
	~CircleComponent();

	// Gets the Entity2D owner
	// @return - Entity2D* for the owner
	Entity2D* GetOwner() { return mOwner2D; }

	// Gets the the entity's collision circle
	// @return - const Cirlce2D& for the circle
	const Circle2D& GetCircle() const { return mCircle; }

	// Gets the center of the circle box
	// @return - const glm::vec2& for the center position
	const glm::vec2& GetCenter() const { return mOwner2D->GetPosition(); }

	// Sets the circle's radius
	// @param - float for the new radius
	void SetRadius(float r) { mCircle.radius = r; }

private:
	// Pointer to entity 2D
	Entity2D* mOwner2D;

	// Circle collision
	Circle2D mCircle;
};


class OBBComponent2D : public CollisionComponent
{
public:
	OBBComponent2D(Entity2D* owner, Physics* physics, BodyType bodyType = BodyType::Dynamic);
	~OBBComponent2D();

	// Override Update for collision: 
	// Updates the oriented bounding box collision location and rotation (converts to radians here) based on owner's position
	// @param - float for delta time
	void Update(float deltaTime) override;

	// Gets the Entity2D owner
	// @return - Entity2D* for the owner
	Entity2D* GetOwner() { return mOwner2D; }

	// Gets the center of the 2D OBB box
	// @return - const glm::vec2& for the center position
	const glm::vec2& GetCenter() const { return mOwner2D->GetPosition(); }

	// Gets the rotation of the 2D OBB in radians
	// @return - float for the single float rotation
	float GetRotation() const { return mOwner2D->GetRotation(); }

	// Sets the OBB half extents (stores half width and half height)
	// @param - const glm::vec2& for the width and height of the box
	void SetSize(const glm::vec2& size) { mOBB.halfExtents = size * 0.5f; }

private:
	// Pointer to entity 2D
	Entity2D* mOwner2D;

	// OBB collision box
	OBB_2D mOBB;
};
