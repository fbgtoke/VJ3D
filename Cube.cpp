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

Cube::Cube() {}
Cube::Cube(const glm::vec4& color) : mColor(color) {}

void Cube::setColor(const glm::vec4& color) { mColor = color; }
glm::vec4 Cube::getColor() const { return mColor; }
