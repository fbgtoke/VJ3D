#include "Cube.h"

const GLfloat Cube::kSize = 0.2f;

const GLfloat Cube::kVertices[] = {
	-1.0f, -1.0f, -1.0f, // Back face
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,

	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,

	 1.0f,  1.0f, -1.0f, // Right face
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f, // Front face
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,

	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f, // Left face
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f,  1.0f, -1.0f, // Top face
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,

	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f, // Bottom face
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f
};

const GLfloat Cube::kNormals[] = {
	 0.0f,  0.0f, -1.0f, // Back face
	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,

	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,
	 0.0f,  0.0f, -1.0f,

	 1.0f,  0.0f,  0.0f, // Right face
	 1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f,

	 1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f,
	 1.0f,  0.0f,  0.0f,

	 0.0f,  0.0f,  1.0f, // Front face 
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,

	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,
	 0.0f,  0.0f,  1.0f,

	-1.0f,  0.0f,  0.0f, // Left face
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,

	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,
	-1.0f,  0.0f,  0.0f,

	 0.0f,  1.0f,  0.0f, // Top face
	 0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,

	 0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,
	 0.0f,  1.0f,  0.0f,

	 0.0f, -1.0f,  0.0f, // Bottom face
	 0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,

	 0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,
	 0.0f, -1.0f,  0.0f,
};

Cube::Cube(ShaderProgram& program)
	: mShaderProgram(program) {}

Cube::~Cube() {}

void Cube::init() {
	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	mScale = 1.0f;

	initShaders();
}

void Cube::render(const glm::mat4& parentTransform) {
	glm::mat4 modelMatrix;

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = parentTransform * modelMatrix;
	modelMatrix = glm::translate(modelMatrix, mPosition);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(kSize, kSize, kSize) * 0.5f * mScale);
	mShaderProgram.setUniformMatrix4f("modelMatrix", modelMatrix);
	mShaderProgram.setUniform4f("color", mColor.x, mColor.y, mColor.z, mColor.w);

	glBindVertexArray(mVao);
	glEnableVertexAttribArray(mPosLocation);
	glEnableVertexAttribArray(mNormLocation);
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glBindVertexArray(0);
}

void Cube::setPosition(const glm::vec3& position) { mPosition = position; }
void Cube::setColor(const glm::vec4& color) { mColor = color; }

void Cube::move(const glm::vec3& movement) { mPosition += movement; }

glm::vec3 Cube::getPosition() const { return mPosition; }
glm::vec4 Cube::getColor() const { return mColor; }

void Cube::initShaders() {
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	
	glGenBuffers(1, &mVboPos);
	glBindBuffer(GL_ARRAY_BUFFER, mVboPos);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), &kVertices[0], GL_STATIC_DRAW);
	mPosLocation = mShaderProgram.bindVertexAttribute("position", 3, 0, nullptr);

	glGenBuffers(1, &mVboNorm);
	glBindBuffer(GL_ARRAY_BUFFER, mVboNorm);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), &kNormals[0], GL_STATIC_DRAW);
	mNormLocation = mShaderProgram.bindVertexAttribute("normal", 3, 0, nullptr);
}

GLfloat Cube::getSize() { return kSize; }