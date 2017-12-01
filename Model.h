#ifndef _MODEL_INCLUDE
#define _MODEL_INCLUDE

#include "utils.h"
#include "Mesh.h"
#include "Texture.h"

class Model {
public:
	Model();
	~Model();

	void init();
	void render();

	void setTexture(std::shared_ptr<Texture> texture);
	void setMesh(std::shared_ptr<Mesh> mesh);

	void setPosition(const glm::vec3& position);
	glm::vec3 getPosition() const;

	void move(const glm::vec3& direction);

private:
	void initVAO();
	GLuint mVAO;
	GLuint mVBO_vertices, mVBO_normals, mVBO_texcoord;
	GLuint mLoc_vertices, mLoc_normals, mLoc_texcoord;

	std::shared_ptr<ShaderProgram> mShaderProgram;

	std::shared_ptr<Texture> mTexture;
	std::shared_ptr<Mesh> mMesh;

	glm::vec3 mPosition;
};

#endif // _MODEL_INCLUDE