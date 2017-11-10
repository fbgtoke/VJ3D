#include "Cursor.h"

Cursor::Cursor(Model& parent, ShaderProgram& program)
	: mParentModel(parent), Model(program) {}

Cursor::~Cursor() {}

void Cursor::init() {
	Model::init();

	setDimensions(glm::ivec3(1, 1, 1));
	setCubeColor(0, 0, 0, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	mCurrentIndex = glm::vec3(0.0f, 0.0f, 0.0f);
	
	setPosition(mParentModel.getPosition());
}

void Cursor::update(int deltaTime) {}

void Cursor::render() {
	mShaderProgram.setUniformBool("enableLight", false);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	Model::render();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	mShaderProgram.setUniformBool("enableLight", true);	
}

glm::mat4 Cursor::getTransform() const {
	glm::mat4 modelTransform;
	modelTransform = glm::mat4(1.0f);
	modelTransform = glm::translate(modelTransform, getPosition());
	modelTransform = mParentModel.getTransform() * modelTransform;

	return modelTransform;
}

void Cursor::move(const glm::vec3& movement) {
	glm::vec3 dimensions = mParentModel.getDimensions();
	mCurrentIndex = glm::mod(mCurrentIndex + dimensions + movement, dimensions);

	glm::vec3 position;
	position.x = mCurrentIndex.x * Cube::kSize;
	position.y = mCurrentIndex.y * Cube::kSize * (-1.0f);
	position.z = mCurrentIndex.z * Cube::kSize * (-1.0f);

	setPosition(position);
}

glm::vec3 Cursor::getCurrentIndex() const { return mCurrentIndex; }

void Cursor::toggleCube() {
	glm::vec4 color;
	color = mParentModel.getCubeColor(mCurrentIndex.x, mCurrentIndex.y, mCurrentIndex.z);

	if (color.w > 0.0f) color.w = 0.0f;
	else color.w = 1.0f;

	mParentModel.setCubeColor(mCurrentIndex.x, mCurrentIndex.y, mCurrentIndex.z, color);
}