#include "Text.h"
#include <iostream>
#include <freetype/freetype.h>
#include <ft2build.h>

Text::Text() :
	mTextShader(nullptr)
{
}

Text::~Text()
{
	std::cout << "Deleted Text\n";
}

void Text::LoadFont(const std::string& fontFileName, unsigned int fontSize)
{
	// Clear any previously loaded characters
	mCharacterMap.clear();

	// Initialize and load FreeType library
	FT_Library freeType;

	if (FT_Init_FreeType(&freeType))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
	}

	// Load the FreeType font face
	FT_Face face;
	if (FT_New_Face(freeType, fontFileName.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font\n";
	}

	// Set font size
	FT_Set_Pixel_Sizes(face, 0, fontSize);

	// TODO: Disable byte alignment???

	for (unsigned char c = 0; c < 128; ++c)
	{
		// Load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph: " << c << "\n";
			continue;
		}




	}



	// De-allocate FreeType memory
	FT_Done_Face(face);
	FT_Done_FreeType(freeType);
}

void Text::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color)
{

}
