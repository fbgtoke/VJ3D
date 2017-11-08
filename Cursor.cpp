#include "Cursor.h"

Cursor::Cursor(const Model& model, ShaderProgram& program)
	: mModel(model), Cube(program) {}

Cursor::~Cursor() {}

void Cursor::init() {
	Cube::init();

	setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	mCurrentIndex = glm::vec3(1.0f, 0.0f, 0.0f);
	setPosition(mCurrentIndex * Cube::getSize());

	mScale *= 1.2f;
}

void Cursor::render() {
	mShaderProgram.setUniformBool("enableLight", false);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	Cube::render(mModel.getTransform());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mShaderProgram.setUniformBool("enableLight", true);	
}

void Cursor::move(const glm::vec3& movement) {
	glm::vec3 dimensions = mModel.getDimensions();
	mCurrentIndex = glm::mod(mCurrentIndex + dimensions + movement, dimensions);

	setPosition(mCurrentIndex * Cube::getSize());
}