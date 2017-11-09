#include "Model.h"
#include "Game.h"

const int Model::kCubesPerTile = 1;

Model::Model(ShaderProgram& program)
	: mShaderProgram(program) {}
	
Model::~Model() {
	mCubes.clear();
}

void Model::init() {
	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	setDimensions(glm::ivec3(3, 2, 1));
}

void Model::update(int deltaTime) {}

void Model::render() {
	glm::mat4 modelMatrix = getTransform();

	for (int i = 0; i < mDimensions.x * mDimensions.y * mDimensions.z; ++i)
		if (mCubes[i].getColor().w != 0.0f)
			mCubes[i].render(modelMatrix);
}

void Model::setDimensions(const glm::ivec3& dimensions) {
	mDimensions = dimensions * kCubesPerTile;

	mCubes = std::vector<Cube> (mDimensions.x * mDimensions.y * mDimensions.z, Cube(mShaderProgram));
	for (int i = 0; i < mDimensions.x * mDimensions.y * mDimensions.z; ++i) mCubes[i].init();

	moveCubesToRelativePosition();
}

glm::ivec3 Model::getDimensions() const { return mDimensions; }

glm::mat4 Model::getTransform() const {
	glm::vec3 modelOffset = glm::vec3(
		(mDimensions.x - 1)  * Cube::getSize() * -0.5f,
		(mDimensions.y - 1) * Cube::getSize() *  0.5f,
		(mDimensions.z - 1)  * Cube::getSize() *  0.5f
	);

	glm::mat4 modelMatrix;
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, mPosition);
	modelMatrix = glm::rotate(modelMatrix, mRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, mRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, mRotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = glm::translate(modelMatrix, modelOffset);

	return modelMatrix;
}

void Model::setPosition(const glm::vec3& position) { mPosition = position; }

void Model::move(const glm::vec3& movement) { mPosition += movement; }

void Model::rotateX(float angle) { mRotation.x += angle; }
void Model::rotateY(float angle) { mRotation.y += angle; }
void Model::rotateZ(float angle) { mRotation.z += angle; }

glm::vec3 Model::getPosition() const { return mPosition; }

void Model::setCubePosition(const glm::ivec3& index, const glm::vec3& position) {
	mCubes[getIndex(index)].setPosition(position);
}

void Model::setCubeColor(const glm::ivec3& index, const glm::vec4& color) {
	setCubeColor(getIndex(index), color);
}

void Model::setCubeColor(int index, const glm::vec4& color) {
	mCubes[index].setColor(color);
}

glm::vec4 Model::getCubeColor(const glm::ivec3& index) const {
	return mCubes[getIndex(index)].getColor();
}

void Model::loadFromFile(const std::string& filename) {
	glm::ivec3 index;
	glm::vec4  color;

	std::ifstream stream;
	stream.open(filename);
	if (stream.is_open()) {
		stream >> index.x >> index.y >> index.z;
		setDimensions(index);

		for (int i = 0; i < mDimensions.x * mDimensions.y * mDimensions.z; ++i) {
			stream >> color.x >> color.y >> color.z >> color.w;
			setCubeColor(i, color);
		}

		stream.close();
	} else {
		std::cout << "Could not open " << filename << std::endl;
	}
}

void Model::saveToFile(const std::string& filename) {
	glm::ivec3 index;
	glm::vec4 color;

	std::ofstream stream;
	stream.open(filename);
	if (stream.is_open()) {
		stream << mDimensions.x << " " << mDimensions.y << " " << mDimensions.z << std::endl;

		for (int z = 0; z < mDimensions.z; ++z) {
			for (int y = 0; y < mDimensions.y; ++y) {
				for (int x = 0; x < mDimensions.x; ++x) {
					index = glm::ivec3(x, y, z);
					color = getCubeColor(index);
					stream
						<< color.x << " "
						<< color.y << " "
						<< color.z << " "
						<< color.w << std::endl;
				}
			}
		}

		stream.close();
	} else {
		std::cout << "Could not open " << filename << std::endl;
	}
}

void Model::moveCubesToRelativePosition() {
	glm::vec3 position;

	for (int y = 0; y < mDimensions.y; ++y) {
		for (int x = 0; x < mDimensions.x; ++x) {
			for (int z = 0; z < mDimensions.z; ++z) {
				position.x = x * Cube::getSize();
				position.y = y * Cube::getSize() * (-1.0f);
				position.z = z * Cube::getSize() * (-1.0f);

				setCubePosition(glm::ivec3(x, y, z), position);
			}
		}
	}
}

int Model::getIndex(int x, int y, int z) const {
	return
		y * mDimensions.x * mDimensions.z +
		x * mDimensions.z +
		z;
}

int Model::getIndex(const glm::ivec3& i) const { return getIndex(i.x, i.y, i.z); }