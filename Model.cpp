#include "Model.h"
#include "Game.h"

const int Model::kCubesPerTile = 16;

Model::Model(ShaderProgram& program)
	: mShaderProgram(program) {}
	
Model::~Model() {
	
	free(mCubes);	
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@(*(@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&&&&&&&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&***@@@@@@@@@@@@@@********@@@@@@@@@@******(@@@@@@@@@@@@***&@@@
//	@@*        @@@@@@@@@@        .@@@@@@@@@,         @@@@@@@@(       @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.   @@@@@@@@@@@@@        @@@@@@@@@@         @@@@@@@@@@   %@@@
//	@@@@@@@@   @@@@@@@@@    @@@    @@@@@@@@,   @@%   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   %@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@@@
//	@@*       ,@@@@@@@@&   @@@@@   @@@@@@@@,   @@%   @@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@(  #&   @@@@@@@@@@@@        @@@@@@@@@@      .%@@@@@@@@@@@   %@@@
//	@@*   @@@@@@@@@@@@@@    @@&    @@@@@@@@,   @@%   @@@@@@@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@,  .@@@@@@@@@@@   %@@@@@@@@@@@@@@@@@@&   @@@@@@@@@@@   %@@@
//	@@*   @@@@@@@@@@@@@@@.       /@@@@@@@@@,   @@%   @@@@@@@@@,     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.         *@@@@@@@@@@        @@@@@@@@@@@@@@&   @@@@@@@@@@@   %@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@&@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@#,,,,,,,,,,,,,,,,************************************////////////%@@@@@@@@@@@@@@@@@@@@@(************,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,................@@@@
//	@@@@@@@&&%##(//**,,,..................................................,@@@@@@@@@@@@@@@@@                                                   ...,,*//(#%%&@@@@@@@
//	@@@@@@@@@@@.....................,,,**/((#%%&&&@@@@@@@@@@@@@@@@&&&....../@@%(((#%   *@@@      .%&&@@@@@@@@@@@@@@@@&&%%##(/**,,...                    @@@@@@@@@@@
//	@@@@@@@@@@@@@@@@&@@@@@@@@@@&%#(**,.......................@#...........@&((((((#%      *@            &@                       ..,/(%&&@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@%,....................,*/(#%&@@@@@@@@@@&&@@@&,.....@((((((((#%        @      .&@@%%&@@@@@@@@@@&%#/*,.                    ,&@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@*.,*/#&@@@@@@/*,.................@(........@(((((((((#%         @         &&                 .,/#&@@@@@@%#*.. *@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@#*,...............,*#%&@@@@@&%(*,%@%....,@(((((((((#%         %,    .@@*.,/%&@@@@@&%(,.               .,#@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%(*...............@....,*.*@(((((((((#%         %, .,.   ,@               ./%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%(**@/.....@(((((((((#%         @      %@.,(%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@/...@((((((((#%        &*   &@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%((((((#%      .@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@&((((#%    /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//	@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

}

void Model::init() {
	mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	mRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	initShaders();
}

void Model::update(int deltaTime) {}

void Model::render() {
	for (int x = 0; x < mDimensions.x; ++x)
		for (int y = 0; y < mDimensions.y; ++y)
			for (int z = 0; z < mDimensions.z; ++z)
				renderCube(x, y, z);
}

void Model::setDimensions(const glm::ivec3& dimensions) {
	mDimensions = dimensions * kCubesPerTile;
	mCubes = (Cube*) malloc(mDimensions.x * mDimensions.y * mDimensions.z * sizeof(Cube));
}

glm::ivec3 Model::getDimensions() const { return mDimensions; }

glm::mat4 Model::getTransform() const {
	glm::vec3 modelOffset = glm::vec3(
		(mDimensions.x - 1)  * Cube::kSize * -0.5f,
		(mDimensions.y - 1)  * Cube::kSize *  0.5f,
		(mDimensions.z - 1)  * Cube::kSize *  0.5f
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

float Model::getRotationX() const { return mRotation.x; }
float Model::getRotationY() const { return mRotation.y; }
float Model::getRotationZ() const { return mRotation.z; }

glm::vec3 Model::getCubePosition(int x, int y, int z) const {
	glm::vec3 position;
	position.x = x * Cube::kSize;
	position.y = y * Cube::kSize * (-1.0f);
	position.z = z * Cube::kSize * (-1.0f);
	
	return position;
}

void Model::setCubeColor(int x, int y, int z, const glm::vec4& color) {
	mCubes[getIndex(x, y, z)].setColor(color);
}

glm::vec4 Model::getCubeColor(int x, int y, int z) const {
	return mCubes[getIndex(x, y, z)].getColor();
}

void Model::loadFromFile(const std::string& filename) {
	glm::ivec3 index;
	glm::vec4  color;

	std::ifstream stream;
	stream.open(filename);
	if (stream.is_open()) {
		stream >> index.x >> index.y >> index.z;
		setDimensions(index);

		for (int x = 0; x < mDimensions.x; ++x) {
			for (int y = 0; y < mDimensions.y; ++y) {
				for (int z = 0; z < mDimensions.z; ++z) {
					stream >> color.x >> color.y >> color.z >> color.w;
						setCubeColor(x, y, z, color);
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
		stream << mDimensions.x << " " << mDimensions.y << " " << mDimensions.z << std::endl;

		for (int z = 0; z < mDimensions.z; ++z) {
			for (int y = 0; y < mDimensions.y; ++y) {
				for (int x = 0; x < mDimensions.x; ++x) {
					index = glm::ivec3(x, y, z);
					color = getCubeColor(x, y, z);
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

void Model::initShaders() {
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	
	glGenBuffers(1, &mVboPos);
	glBindBuffer(GL_ARRAY_BUFFER, mVboPos);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), &Cube::kVertices[0], GL_STATIC_DRAW);
	mPosLocation = mShaderProgram.bindVertexAttribute("position", 3, 0, nullptr);

	glGenBuffers(1, &mVboNorm);
	glBindBuffer(GL_ARRAY_BUFFER, mVboNorm);
	glBufferData(GL_ARRAY_BUFFER, 36 * 3 * sizeof(GLfloat), &Cube::kNormals[0], GL_STATIC_DRAW);
	mNormLocation = mShaderProgram.bindVertexAttribute("normal", 3, 0, nullptr);
}

void Model::renderCube(int x, int y, int z) {
	glm::vec3 position = getCubePosition(x, y, z);
	glm::vec4 color = getCubeColor(x, y, z);

	if (color.w < 0.5f) return;

	glm::mat4 parentTransform = getTransform();

	glm::mat4 modelMatrix;
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = parentTransform * modelMatrix;
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(Cube::kSize) * 0.5f);
	mShaderProgram.setUniformMatrix4f("modelMatrix", modelMatrix);
	mShaderProgram.setUniform4f("color", color.x, color.y, color.z, color.w);

	glBindVertexArray(mVao);
	glEnableVertexAttribArray(mPosLocation);
	glEnableVertexAttribArray(mNormLocation);
	glDrawArrays(GL_TRIANGLES, 0, 36 * 3);
	glBindVertexArray(0);
}

int Model::getIndex(int x, int y, int z) const {
	return
		x * mDimensions.y * mDimensions.z +
		y * mDimensions.z +
		z;
}