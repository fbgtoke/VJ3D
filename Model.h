#ifndef _MODEL_INCLUDE
#define _MODEL_INCLUDE

#include "Cube.h"

class Model {
public:
	Model(ShaderProgram& program);
	~Model();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();

	void setTiles(const glm::ivec3& tiles);
	glm::ivec3 getTiles() const;

	void setDimensions(const glm::ivec3& dimensions);
	glm::ivec3 getDimensions() const;

	virtual glm::mat4 getTransform() const;

	void setPosition(const glm::vec3& position);
	virtual void move(const glm::vec3& movement);

	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	void scale(float factor);

	void setCubeColor(int x, int y, int z, const glm::vec4& color);

	glm::vec3 getPosition() const;

	float getRotationX() const;
	float getRotationY() const;
	float getRotationZ() const;

	glm::vec3 getCubePosition(int x, int y, int z) const;
	glm::vec4 getCubeColor(int x, int y, int z) const;	

	void loadFromFile(const std::string& filename);
	void saveToFile(const std::string& filename);

protected:	
	glm::vec3 mPosition;
	glm::vec3 mRotation;
	float mScale;

	static const int kCubesPerTile;
	glm::ivec3 mDimensions;
	glm::ivec3 mTiles;

	ShaderProgram& mShaderProgram;
	GLuint mVao;
	GLuint mVboPos, mVboNorm;
	GLint mPosLocation, mNormLocation;
	void initShaders();
	void renderCube(int x, int y, int z);

	int getIndex(int x, int y, int z) const;

	Cube* mCubes;
};

#endif // _MODEL_INCLUDE