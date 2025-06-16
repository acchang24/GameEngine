#include "Text.h"
#include <iostream>
#include <freetype/freetype.h>
#include <ft2build.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer2D.h"
#include "Shader.h"
#include "Texture.h"

Text::Text(Renderer2D* renderer) :
	mRenderer(renderer),
	mShader(nullptr),
	mVAO(0),
	mVBO(0),
	mMaxBearingY(0)
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	// Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	// UV
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Text::~Text()
{
	std::cout << "Deleted Text\n";

	ClearCharacters();
}

void Text::LoadFont(const std::string& fontFileName, unsigned int fontSize)
{
	ClearCharacters();

	// Initialize and load FreeType library
	FT_Library freeType;

	if (FT_Init_FreeType(&freeType))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
		return;
	}

	// Load the FreeType font face
	FT_Face face;
	if (FT_New_Face(freeType, fontFileName.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font\n";
		return;
	}

	// Set font size
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// Disable byte alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; ++c)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph: " << c << "\n";
			continue;
		}

		Texture* texture = new Texture(TextureType::Font);
		texture->BindTexture();
		texture->GenerateTexture(GL_TEXTURE_2D, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer, 
			false, false, 1, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);

		int bearingY = face->glyph->bitmap_top;
		if (bearingY > mMaxBearingY)
		{
			mMaxBearingY = bearingY;
		}
		Character character = {
			glm::ivec2(static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows)),
			glm::ivec2(static_cast<int>(face->glyph->bitmap_left), static_cast<int>(face->glyph->bitmap_top)),
			texture,
			static_cast<unsigned int>(face->glyph->advance.x)
		};

		mCharacterMap[c] = character;
	}

	// De-allocate FreeType memory
	FT_Done_Face(face);
	FT_Done_FreeType(freeType);

	// Restore back to 4 byte alignment
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

void Text::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color)
{
	if (mShader)
	{
		mShader->SetActive();

		mShader->SetMat4("projection", mRenderer->GetProjection());

		for (auto& character : text)
		{
			Character c = mCharacterMap[character];

			Texture* texture = c.texture;

			float xpos = x + c.bearing.x * scale;
			float ypos = y + (mMaxBearingY - c.bearing.y) * scale;

			float w = c.size.x * scale;
			float h = c.size.y * scale;
			// update VBO for each character
			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 1.0f },
				{ xpos,     ypos,       0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 0.0f },

				{ xpos,     ypos + h,   0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 0.0f },
				{ xpos + w, ypos + h,   1.0f, 1.0f }
			};

			mShader->SetVec3("textColor", color);
			mShader->SetInt("text", texture->GetTextureUnit());
			
			texture->BindTexture();

			glBindVertexArray(mVAO);

			// update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += (c.advanceOffset >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}
}

void Text::ClearCharacters()
{
	for (auto& c : mCharacterMap)
	{
		delete c.second.texture;
	}
	mCharacterMap.clear();
}
