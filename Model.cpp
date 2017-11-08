#include "Model.h"
#include "Game.h"

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

void Model::update(int deltaTime) {
	if (Game::instance().getSpecialKeyPressed(GLUT_KEY_UP))
		move(glm::vec3(0.0f, 1.0f, 0.0f));
	if (Game::instance().getSpecialKeyPressed(GLUT_KEY_DOWN))
		move(glm::vec3(0.0f, -1.0f, 0.0f));
	if (Game::instance().getSpecialKeyPressed(GLUT_KEY_LEFT))
		move(glm::vec3(-1.0f, 0.0f, 0.0f));
	if (Game::instance().getSpecialKeyPressed(GLUT_KEY_RIGHT))
		move(glm::vec3(1.0f, 0.0f, 0.0f));

	if (Game::instance().getKeyPressed('w'))
		mRotation.x += M_PI / 4.0f;
	if (Game::instance().getKeyPressed('s'))
		mRotation.x -= M_PI / 4.0f;
	if (Game::instance().getKeyPressed('a'))
		mRotation.y += M_PI / 4.0f;
	if (Game::instance().getKeyPressed('d'))
		mRotation.y -= M_PI / 4.0f;
}

void Model::render() {
	glm::mat4 modelMatrix = getTransform();

	for (int y = 0; y < mHeight; ++y)
		for (int x = 0; x < mWidth; ++x)
			for (int z = 0; z < mDepth; ++z)
				if (mCubes[y][x][z].getColor().w != 0.0f)
					mCubes[y][x][z].render(modelMatrix);
}

void Model::setDimensions(const glm::ivec3& dimensions) {
	mWidth  = dimensions.x;
	mHeight = dimensions.y;
	mDepth  = dimensions.z;

	mCubes = std::vector<std::vector<std::vector<Cube>>>(
		mHeight, std::vector<std::vector<Cube>> (
			mWidth, std::vector<Cube> (
				mDepth, Cube(mShaderProgram)
			)
		)
	);

	for (int y = 0; y < mHeight; ++y)
		for (int x = 0; x < mWidth; ++x)
			for (int z = 0; z < mDepth; ++z)
				mCubes[y][x][z].init();

	moveCubesToRelativePosition();
}

glm::ivec3 Model::getDimensions() const { return glm::ivec3(mWidth, mHeight, mDepth); }

glm::mat4 Model::getTransform() const {
	glm::vec3 modelOffset = glm::vec3(
		(mWidth - 1)  * Cube::getSize() * -0.5f,
		(mHeight - 1) * Cube::getSize() *  0.5f,
		(mDepth - 1)  * Cube::getSize() *  0.5f
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
	mCubes[index.y][index.x][index.z].setPosition(position);
}

void Model::setCubeColor(const glm::ivec3& index, const glm::vec4& color) {
	mCubes[index.y][index.x][index.z].setColor(color);
}

glm::vec4 Model::getCubeColor(const glm::ivec3& index) const {
	return mCubes[index.y][index.x][index.z].getColor();
}

void Model::loadFromFile(const std::string& filename) {
	glm::ivec3 index;
	glm::vec4  color;

	std::ifstream stream;
	stream.open(filename);
	if (stream.is_open()) {
		stream >> index.x >> index.y >> index.z;
		setDimensions(index);

		for (int z = 0; z < mDepth; ++z) {
			for (int y = 0; y < mHeight; ++y) {
				for (int x = 0; x < mWidth; ++x) {
					stream >> color.x >> color.y >> color.z >> color.w;
					setCubeColor(glm::ivec3(x, y, z), color);
				}
			}
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
		stream << mWidth << " " << mHeight << " " << mDepth << std::endl;

		for (int z = 0; z < mDepth; ++z) {
			for (int y = 0; y < mHeight; ++y) {
				for (int x = 0; x < mWidth; ++x) {
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

	for (int y = 0; y < mHeight; ++y) {
		for (int x = 0; x < mWidth; ++x) {
			for (int z = 0; z < mDepth; ++z) {
				position.x = x * Cube::getSize();
				position.y = y * Cube::getSize() * (-1.0f);
				position.z = z * Cube::getSize() * (-1.0f);

				setCubePosition(glm::ivec3(x, y, z), position);
			}
		}
	}
}