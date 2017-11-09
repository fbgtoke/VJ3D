#ifndef _MODEL_INCLUDE
#define _MODEL_INCLUDE

#include "Cube.h"

class Model {
public:
	Model(ShaderProgram& program);
	~Model();

	void init();
	void update(int deltaTime);
	void render();

	void setDimensions(const glm::ivec3& dimensions);
	glm::ivec3 getDimensions() const;

	glm::mat4 getTransform() const;

	void setPosition(const glm::vec3& position);
	void move(const glm::vec3& movement);

	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	glm::vec3 getPosition() const;

	void setCubePosition(const glm::ivec3& index, const glm::vec3& position);

	void setCubeColor(const glm::ivec3& index, const glm::vec4& color);
	void setCubeColor(int index, const glm::vec4& color);

	glm::vec4 getCubeColor(const glm::ivec3& index) const;

	void loadFromFile(const std::string& filename);
	void saveToFile(const std::string& filename);

private:
	ShaderProgram& mShaderProgram;
	
	glm::vec3 mPosition;
	glm::vec3 mRotation;

	static const int kCubesPerTile;
	glm::ivec3 mDimensions;
	std::vector<Cube> mCubes;
	void moveCubesToRelativePosition();

	int getIndex(int x, int y, int z) const;
	int getIndex(const glm::ivec3& i) const;
};

#endif // _MODEL_INCLUDE