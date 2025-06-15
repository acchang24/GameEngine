#include "Renderer2D.h"
#include <algorithm>
#include <iostream>
#include <ft2build.h>
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
	mFont(),
	mShader(nullptr),
	mVertexBuffer(nullptr),
	mWidth(width),
	mHeight(height)
{
}

Renderer2D::~Renderer2D()
{
	std::cout << "Deleted SpriteRenderer\n";

	delete mVertexBuffer;
}

bool Renderer2D::Init()
{
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

	
	if (FT_Init_FreeType(&mFont))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return false;
	}

	return true;
}

void Renderer2D::DrawSprites()
{
	if (mShader)
	{
		mShader->SetActive();

		for (auto sprite : mSprites)
		{
			if (sprite->IsVisible())
			{
				glm::mat4 model = glm::mat4(1.0f);

				Entity2D* e = dynamic_cast<Entity2D*>(sprite->GetEntity());

				Texture* tex = sprite->GetCurrentSprite();

				glm::vec2 size = e->GetSize();

				// Translate position
				model = glm::translate(model, glm::vec3(e->GetPosition(), 0.0f));

				// Rotate
				model = glm::rotate(model, glm::radians(e->GetRotation()), glm::vec3(0.0f, 0.0f, 1.0f));

				// Scale
				model = glm::scale(model, glm::vec3(e->GetScale() * size, 1.0f));

				// Send model and projection matrix to shader
				mShader->SetMat4("model", model);

				mShader->SetMat4("projection", mProjection);

				mShader->SetInt("sprite", tex->GetTextureUnit());

				// Bind the texture
				tex->BindTexture();

				// Draw
				mVertexBuffer->Draw();
			}
		}
	}
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
