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

	// Loops through sprite component vector and draws each sprite
	void DrawSprites();

	// Draws rectangle to screen
	// @param - float for x position
	// @param - float for y position
	// @param - float for width
	// @param - float for height
	// @param - const glm::vec3& for color of rectangle
	void DrawRect(float x, float y, float width, float height, const glm::vec4& color);

	// Draws rectangle outline
	// @param - float for x position
	// @param - float for y position
	// @param - float for width
	// @param - float for height
	// @param - const glm::vec3& for color of rectangle
	// @param - float for optional thickness (defaults to 1.0f)
	void DrawRectOutline(float x, float y, float width, float height, const glm::vec4& color, float thickness = 1.0f);
	
	// Renders a string of text using pre-compiled font textures
	// @param - const std::string& for the text
	// @param - float for x position
	// @param - float for y position
	// @param - float for the scale
	// @param - const glm::vec3& for the color of the font (optional, defaults to white)
	void DrawText(const std::string& text, float x, float y, float size, const glm::vec3& color);

	// Adds a sprite to the sprite renderer, sorting it by draw order (lower means further back)
	// @param - SpriteComponent* for the new sprite
	void AddSprite(SpriteComponent* sprite);

	// Removes a sprite from the sprite renderer
	void RemoveSprite(SpriteComponent* sprite);

	// Gets the text renderer
	// @return - Text* for the text renderer
	Text* GetTextRenderer() { return mTextRenderer; }

	// Gets the renderer projection matrix
	// @return - const glm::mat4& for the projection
	const glm::mat4& GetProjection() const { return mProjection; }

	// Gets the renderer width
	// @return - float for width
	float GetWidth() const { return mWidth; }

	// Gets the renderer height
	// @return - float for height
	float GetHeight() const { return mHeight; }

	// Sets the sprite renderer's shader
	// @param - Shader* for the new shader
	void SetSpriteShader(Shader* shader) { mSpriteShader = shader; }

	// Sets the text renderer's shader
	// @param - Shader* for the new shader
	void SetTextShader(Shader* shader) { mTextRenderer->SetShader(shader);}

	// Sets the ui shader
	// @param - Shader* for the new shader
	void SetUIBoxShader(Shader* shader) { mUIBoxShader = shader; }

private:
	// Array of sprites
	std::vector<SpriteComponent*> mSprites;

	// Projection matrix used for 2D rendering
	glm::mat4 mProjection;

	// Text renderer
	Text* mTextRenderer;

	// Shader used to render sprites
	Shader* mSpriteShader;

	// Shader used for ui
	Shader* mUIBoxShader;

	// Vertex buffer to represent the quad vertices that this frame buffer can draw to
	VertexBuffer* mVertexBuffer;

	// Width of renderer
	float mWidth;

	// Height of renderer
	float mHeight;
};
