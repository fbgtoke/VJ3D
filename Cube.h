#ifndef _CUBE_INCLUDE
#define _CUBE_INCLUDE

#include "utils.h"

class Cube {
public:
	Cube(ShaderProgram& program);
	~Cube();

	virtual void init();
	virtual void render(const glm::mat4& parentTransform);

	void setPosition(const glm::vec3& position);
	void setColor(const glm::vec4& color);

	void move(const glm::vec3& movement);

	glm::vec3 getPosition() const;
	glm::vec4 getColor() const;

	static GLfloat getSize();

protected:
	static const GLfloat kSize;
	static const GLfloat kVertices[];
	static const GLfloat kNormals[];

	ShaderProgram& mShaderProgram;
	GLuint mVao;
	GLuint mVboPos, mVboNorm;
	GLint mPosLocation, mNormLocation;
	void initShaders();

	glm::vec3 mPosition;
	glm::vec4 mColor;

	float mScale;
};

#endif // _CUBE_INCLUDE