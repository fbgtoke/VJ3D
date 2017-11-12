#ifndef _TEXT_INCLUDE
#define _TEXT_INCLUDE

#include "utils.h"

#include "Texture.h"
#include "TexturedQuad.h"

#define ATLAS_FONT_SIZE 64
#define NUM_CHARACTERS (128 - 32)


struct CharMetrics {
	int tx, ty; 		// Pos x, y inside texture in pixels
	int sx, sy; 		// Size of char in pixels
	int ax, ay; 		// Advance for each character in a string sequence
	int bl, bt; 		// Bitmap left and top adjusting
};


// Using Freetype the Text class is capable of building a texture atlas
// for a given font and size. Combining with TexturedQuad allows it 
// to render text strings


class Text
{

public:
	Text(ShaderProgram& program);
	~Text();
	
	bool init(const char *filename);
	void destroy();

	void setString(const std::string& str);
	void setPosition(const glm::vec2& position);
	void setSize(int size);
	void setColor(const glm::vec4& color);

	std::string getString() const;
	glm::vec2 getPosition() const;
	int getSize() const;
	glm::vec4 getColor() const;

	int getFontSize() const;
	void render();
	
private:
	bool extractCharSizes(int *maxCharWidth, int *maxCharHeight);
	void createTextureAtlas();
	
private:
	int fontSize, textureSize, maxCharWidth, maxCharHeight;
	FT_Face face;
	CharMetrics chars[NUM_CHARACTERS];
	Texture textureAtlas;
	ShaderProgram& program;
	TexturedQuad *quad;

	static bool bLibInit;
	static FT_Library library;

	std::string mString;
	glm::vec2 mPosition;
	int mSize;
	glm::vec4 mColor;
};


#endif // _TEXT_INCLUDE