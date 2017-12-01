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

private:
	GLuint mVAO;
	GLuint mVBO_vertices, mVBO_normals, mVBO_texcoord;
	GLuint mLoc_vertices, mLoc_normals, mLoc_texcoord;

	std::shared_ptr<ShaderProgram> mShaderProgram;

	Texture mTexture;
	std::shared_ptr<Mesh> mMesh;
};

#endif // _MODEL_INCLUDE