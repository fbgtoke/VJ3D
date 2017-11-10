#ifndef _CUBE_INCLUDE
#define _CUBE_INCLUDE

#include "utils.h"

class Cube {
public:
	Cube();
	Cube(const glm::vec4& color);

	void setColor(const glm::vec4& color);
	glm::vec4 getColor() const;

	void render();

	static const GLfloat kSize;
	static const GLfloat kVertices[];
	static const GLfloat kNormals[];

	glm::vec4 mColor;
};

#endif // _CUBE_INCLUDE