#include "Text.h"
#include <iostream>
#include <freetype/freetype.h>
#include <ft2build.h>
#include "Texture.h"

Text::Text() :
	mTextShader(nullptr)
{
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
		texture->GenerateTexture(GL_TEXTURE_2D, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer, 
			false, false, 1, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);

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

}

void Text::ClearCharacters()
{
	for (auto& c : mCharacterMap)
	{
		delete c.second.texture;
	}
	mCharacterMap.clear();
}
