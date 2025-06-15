#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>


// Struct for character glyph used for text rendering
struct Character
{
	glm::ivec2 size;				// Size (width and height in pixels) of the glyph texture
	glm::ivec2 bearing;				// Offset from baseline to left/top of glyph
	unsigned int textureID;			// Text texture ID
	unsigned int advanceOffset;		// Offset to advance to next glyph
};

class Shader;

// Text class for rendering text displayed by the loaded font
class Text
{
public:
	Text();
	~Text();

	// Loads/sets text font and font size by pixel height
	// @param - const std::string& for the font file name
	// @param - unsigned int for font pixel height (width auto adjusts based on height)
	void LoadFont(const std::string& fontFileName, unsigned int fontSize);

	// Renders a string of text using pre-compiled font textures
	// @param - const std::string& for the text
	// TODO: Add param comments
	// @param - float for the scale
	// @param - const glm::vec3& for the color of the font (optional, defaults to white)
	void RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color = glm::vec3(1.0f));

	// Sets the shader used for text rendering
	// @param - Shader* for the new shader
	void SetShader(Shader* shader) { mTextShader = shader; }

private:
	// Map of char to Character glyph
	std::unordered_map<char, Character> mCharacterMap;

	// Shader used for rendering text
	Shader* mTextShader;
};
