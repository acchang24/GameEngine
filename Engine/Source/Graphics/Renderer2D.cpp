#include "Renderer2D.h"
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Components/SpriteComponent.h"
#include "../Entity/Entity2D.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBuffer.h"

Renderer2D::Renderer2D(float width, float height) :
	mProjection(glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f)),
	mSpriteShader(nullptr),
	mUIBoxShader(nullptr),
	mTextRenderer(nullptr),
	mVertexBuffer(nullptr),
	mWidth(width),
	mHeight(height)
{
	mTextRenderer = new Text(this);

	// Vertex attributes for screen quad that fills the entire screen in Normalized Device Coordinates
	VertexScreenQuad quadVertices[] =
	{
		glm::vec2(-0.5f,  0.5f), glm::vec2(0.0f, 1.0f),
		glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 0.0f),
		glm::vec2(-0.5f, -0.5f), glm::vec2(0.0f, 0.0f),

		glm::vec2(-0.5f,  0.5f), glm::vec2(0.0f, 1.0f),
		glm::vec2(0.5f,  0.5f), glm::vec2(1.0f, 1.0f),
		glm::vec2(0.5f, -0.5f), glm::vec2(1.0f, 0.0f)
	};
	mVertexBuffer = new VertexBuffer(quadVertices, 0, sizeof(quadVertices), 0, sizeof(quadVertices) / sizeof(VertexScreenQuad), 0, VertexLayout::VertexScreenQuad);
}

Renderer2D::~Renderer2D()
{
	std::cout << "Deleted Renderer2D\n";

	delete mTextRenderer;

	delete mVertexBuffer;
}

void Renderer2D::DrawSprites()
{
	if (mSpriteShader)
	{
		mSpriteShader->SetActive();

		for (auto sprite : mSprites)
		{
			if (sprite->IsVisible())
			{
				glm::mat4 model = glm::mat4(1.0f);

				Entity2D* e = static_cast<Entity2D*>(sprite->GetEntity());

				Texture* tex = sprite->GetCurrentSprite();

				glm::vec2 size = e->GetSize();

				// Translate position
				model = glm::translate(model, glm::vec3(e->GetPosition(), 0.0f));

				// Rotate
				model = glm::rotate(model, glm::radians(e->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));

				// Scale
				model = glm::scale(model, glm::vec3(e->GetScale() * size, 1.0f));

				// Send model and projection matrix to shader
				mSpriteShader->SetMat4("model", model);

				mSpriteShader->SetMat4("projection", mProjection);

				if (tex)
				{
					mSpriteShader->SetInt("sprite", tex->GetTextureUnit());

					// Bind the texture
					tex->BindTexture();
				}

				// Draw
				mVertexBuffer->Draw();
			}
		}
	}
}

void Renderer2D::DrawRect(float x, float y, float width, float height, const glm::vec4& color)
{
	if (mUIBoxShader)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(x + width * 0.5f, y + height * 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(width, height, 1.0f));

		mUIBoxShader->SetActive();
		mUIBoxShader->SetMat4("model", model);
		mUIBoxShader->SetMat4("projection", mProjection);
		mUIBoxShader->SetVec4("color", color);
		
		mVertexBuffer->Draw();
	}
}

void Renderer2D::DrawRectOutline(float x, float y, float width, float height, const glm::vec4& color, float thickness)
{
	DrawRect(x, y, width, thickness, color);							// Top
	DrawRect(x, y + height - thickness, width, thickness, color);		// Bottom
	DrawRect(x, y, thickness, height, color);							// Left
	DrawRect(x + width - thickness, y, thickness, height, color);		// Right
}

void Renderer2D::DrawText(const std::string& text, float x, float y, float size, const glm::vec3& color)
{
	mTextRenderer->RenderText(text, x, y, size, color);
}

void Renderer2D::AddSprite(SpriteComponent* sprite)
{
	mSprites.emplace_back(sprite);
	
	// Sort the vector by draw order (smalll to largest so lower sprites get drawn first and will be further back)
	std::sort(mSprites.begin(), mSprites.end(), [](SpriteComponent* a, SpriteComponent* b) {
		return a->GetDrawOrder() < b->GetDrawOrder();
	});
}

void Renderer2D::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	if (iter != mSprites.end())
	{
		mSprites.erase(iter);
	}
}
