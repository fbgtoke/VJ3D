#include "Text.h"
using namespace std;

bool Text::bLibInit = false;
FT_Library Text::library;

Text::Text(ShaderProgram& program)
	: quad(nullptr), program(program) {}

Text::~Text() {
	destroy();
	if(quad != nullptr) {
		quad->free();
		delete quad;
	}
}


bool Text::init(const char *filename) {
	FT_Error error;
	
	if(!bLibInit) {
		error = FT_Init_FreeType(&Text::library);
		if(error)
			return false;
		bLibInit = true;
	}
	error = FT_New_Face(Text::library, filename, 0, &face);
	if(error)
		return false;
	FT_Set_Pixel_Sizes(face, ATLAS_FONT_SIZE, ATLAS_FONT_SIZE);
	
	if(!extractCharSizes(&maxCharWidth, &maxCharHeight))
		return false;
	fontSize = maxCharHeight;
	textureSize = 512;
	if(floor(float(textureSize) / maxCharWidth) * floor(float(textureSize) / maxCharHeight) < (128 - 32))
		textureSize = 1024;
	if(floor(float(textureSize) / maxCharWidth) * floor(float(textureSize) / maxCharHeight) < (128 - 32))
		return false;
	createTextureAtlas();
	
	glm::vec2 geom[2] = {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f)};
	
	quad = TexturedQuad::createTexturedQuad(geom, texCoords, program);

	mString = "";
	mPosition = glm::vec2(0.0f, 0.0f);
	mSize = fontSize;
	mColor = glm::vec4(0, 0, 0, 1);
	
	return true;
}

void Text::destroy() { FT_Done_Face(face); }

void Text::setString(const std::string& str) { mString = str; }
void Text::setPosition(const glm::vec2& position) { mPosition = position; }
void Text::setSize(int size) { mSize = size; }
void Text::setColor(const glm::vec4& color) { mColor = color; }

std::string Text::getString() const { return mString; }
glm::vec2 Text::getPosition() const { return mPosition; }
int Text::getSize() const { return mSize; }
glm::vec4 Text::getColor() const { return mColor; }

int Text::getFontSize() const { return fontSize; }

void Text::render() {
	glm::mat4 projection, modelview;
	glm::vec2 minTexCoord, maxTexCoord;
	
	glm::vec3 position = glm::vec3(mPosition, 1.0f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	program.setUniform4f("color", mColor.r, mColor.g, mColor.b, mColor.a);

	for(unsigned int i = 0; i < mString.length(); i++) {
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, position);
		modelview = glm::scale(modelview, (float(mSize) / fontSize) * glm::vec3(chars[mString[i]-32].sx, chars[mString[i]-32].sy, 0.f));
		program.setUniformMatrix4f("modelview", modelview);
		
		minTexCoord = glm::vec2(float(chars[mString[i]-32].tx) / textureSize, float(chars[mString[i]-32].ty) / textureSize);
		maxTexCoord = glm::vec2(float(chars[mString[i]-32].tx + chars[mString[i]-32].sx) / textureSize, float(chars[mString[i]-32].ty + chars[mString[i]-32].sy) / textureSize);
		
		program.setUniform2f("minTexCoord", minTexCoord.s, minTexCoord.t);
		program.setUniform2f("maxTexCoord", maxTexCoord.s, maxTexCoord.t);
		quad->render(textureAtlas);

		position.x += (float(mSize) / fontSize) * chars[mString[i]-32].ax;
	}

	glDisable(GL_BLEND);
}

bool Text::extractCharSizes(int *maxCharWidth, int *maxCharHeight)
{
	unsigned char c;

	*maxCharWidth = 0;
	*maxCharHeight = 0;
	for(c=32; c<128; c++)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
			return false;
		*maxCharWidth = glm::max(*maxCharWidth, (int)face->glyph->bitmap.width);
		*maxCharHeight = glm::max(*maxCharHeight, (int)face->glyph->bitmap.rows);
	}
	
	return true;
}

void Text::createTextureAtlas()
{
	unsigned char c;
	int x = 0, y = 0;

	textureAtlas.createEmptyTexture(textureSize, textureSize);
	for(c=32; c<128; c++)
	{
		FT_Load_Char(face, c, FT_LOAD_RENDER);
		chars[c-32].tx = x;
		chars[c-32].ty = y;
		chars[c-32].sx = face->glyph->bitmap.width;
		chars[c-32].sy = face->glyph->bitmap.rows;
		chars[c-32].ax = face->glyph->advance.x >> 6;
		chars[c-32].ay = face->glyph->advance.y >> 6;
		chars[c-32].bl = face->glyph->bitmap_left;
		chars[c-32].bt = face->glyph->bitmap_top;
		textureAtlas.loadSubtextureFromGlyphBuffer(face->glyph->bitmap.buffer, x, y, face->glyph->bitmap.width, face->glyph->bitmap.rows);
		x += maxCharWidth;
		if((x + maxCharWidth) >= textureSize)
		{
			x = 0;
			y += maxCharHeight;
		}
	}
	textureAtlas.generateMipmap();
	textureAtlas.setWrapS(GL_CLAMP_TO_EDGE);
	textureAtlas.setWrapT(GL_CLAMP_TO_EDGE);
}



