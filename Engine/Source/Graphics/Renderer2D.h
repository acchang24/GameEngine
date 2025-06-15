#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Text.h"

class Renderer;
class Shader;
class SpriteComponent;
class VertexBuffer;

class Renderer2D
{
public:
	// Renderer2D constructor:
	// @param - float for window width
	// @param - float for window height
	Renderer2D(float width, float height);
	~Renderer2D();

	// Inits Renderer2D: Inits FreeType library
	// @return - bool for if Renderer2D was initialized
	bool Init();

	// Loops through sprite component vector and draws each sprite
	void DrawSprites();

	// Adds a sprite to the sprite renderer, sorting it by draw order (lower means further back)
	// @param - SpriteComponent* for the new sprite
	void AddSprite(SpriteComponent* sprite);

	// Removes a sprite from the sprite renderer
	void RemoveSprite(SpriteComponent* sprite);

	// Sets the sprite renderer's shader
	// @param - Shader* for the new shader
	void SetSpriteShader(Shader* shader) { mSpriteShader = shader; }

	// Sets the text renderer's shader
	// @param - Shader* for the new shader
	void SetTextShader(Shader* shader) { mTextRenderer->SetShader(shader);}

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

	// Text renderer
	Text* mTextRenderer;

	// Shader used to render sprites
	Shader* mSpriteShader;

	// Shader used to render text
	Shader* mTextShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	// Width of renderer
	float mWidth;

	// Height of renderer
	float mHeight;
};
