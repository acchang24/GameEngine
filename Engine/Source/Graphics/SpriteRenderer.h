#pragma once
#include <vector>
#include <glm/glm.hpp>

class Renderer;
class Shader;
class SpriteComponent;
class VertexBuffer;

class SpriteRenderer
{
public:
	// SpriteRenderer constructor:
	// @param - Shader* for the shader used to render sprites
	// @param - float for window width
	// @param - float for window height
	SpriteRenderer(Shader* shader, float width, float height);
	~SpriteRenderer();

	// Loops through sprite component vector and draws each sprite
	void Draw();

	// Adds a sprite to the sprite renderer, sorting it by draw order (lower means further back)
	// @param - SpriteComponent* for the new sprite
	void AddSprite(SpriteComponent* sprite);

	// Removes a sprite from the sprite renderer
	void RemoveSprite(SpriteComponent* sprite);

	// Sets the sprite renderer's shader
	// @param - Shader* for the new shader
	void SetShader(Shader* shader) { mShader = shader; }

	// Gets the renderer width
	// @return - float for width
	float GetWidth() const { return mWidth; }

	// Gets the renderer height
	// @return - float for height
	float GetHeight() const { return mHeight; }

private:
	// Array of sprites
	std::vector<SpriteComponent*> mSprites;

	// Projection matrix used for 2D rendering
	glm::mat4 mProjection;

	// Shader used to render sprites
	Shader* mShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	float mWidth;

	float mHeight;
};
