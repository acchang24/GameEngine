#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include "../Entity/Entity2D.h"

// Enum class for collision shape type
enum class CollisionShapeType
{
	AABB2D,
	Circle,
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
	Kinematic
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

class CollisionComponent : public Component
{
public:
	CollisionComponent(Entity* owner, CollisionShapeType shapeType, BodyType bodyType);
	virtual ~CollisionComponent();

	// Override of Component::Update() for collision. 
	// This is a virtual function (overridable)
	virtual void Update(float deltaTime) override;

	// Intersect check for collisions (overridable)
	// @param - const CollisionComponent* for the other object's collision component
	virtual bool Intersects(const CollisionComponent* other) const;

	// Gets the collision component's shape type
	// @return - CollisionShapeType for the shape type
	CollisionShapeType GetShapeType() const { return mShapeType; }

	// Gets the collision component's body type
	// @return - BodyType for the body type
	BodyType GetBodyType() const { return mBodyType; }

protected:
	// Type of the shape used for collision
	CollisionShapeType mShapeType;

	// Body type of the shape
	BodyType mBodyType;
};

class AABBComponent2D : public CollisionComponent
{
public:
	// AABBComponent2D constructor:
	// @param - Entity2D* for the owner
	// @param - BodyType (optional, defaults to dynamic)
	AABBComponent2D(Entity2D* owner, BodyType bodyType = BodyType::Dynamic);
	~AABBComponent2D();

	// Override Update for collision: 
	// Updates the collision box location based on owner's position
	// @param - float for delta time
	void Update(float deltaTime) override;

	// Override of Component::Intersect for AABB2D
	// @param - const CollisionComponent* for the other component
	bool Intersects(const CollisionComponent* other) const override;

	// Checks to see if this collides with another AABB_2D collision box
	// @param - const AABBComponent2D* for the other collision box
	// @param - bool for if the boxes collide
	bool Intersect(const AABBComponent2D* other) const;

	// Gets the side of this object that collides with another 2d AABB
	// @param - const AABBComponent2D* for the other component's AABB
	// @param - glm::vec2& to provide an offset to push objects back
	CollisionSide GetMinOverlap(const AABBComponent2D* other, glm::vec2& offset);

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
