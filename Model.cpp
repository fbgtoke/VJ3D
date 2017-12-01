#include "Model.h"
#include "Game.h"

Model::Model() {}

Model::~Model() {}

void Model::init() {
	mShaderProgram = Game::instance().getResource().shader("simple");

	mTexture.loadFromFile("models/cactus.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mMesh = Game::instance().getResource().mesh("cactus.obj");

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_vertices);
	glBufferData(GL_ARRAY_BUFFER, mMesh->verticesSize(), mMesh->vertices(), GL_STATIC_DRAW);
	mLoc_vertices = mShaderProgram->bindVertexAttribute("vertex", 3, 0, 0);
	glEnableVertexAttribArray(mLoc_vertices);

	glGenBuffers(1, &mVBO_normals);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_normals);
	glBufferData(GL_ARRAY_BUFFER, mMesh->normalsSize(), mMesh->normals(), GL_STATIC_DRAW);
	mLoc_normals = mShaderProgram->bindVertexAttribute("normal", 3, 0, 0);
	glEnableVertexAttribArray(mLoc_normals);

	glGenBuffers(1, &mVBO_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_texcoord);
	glBufferData(GL_ARRAY_BUFFER, mMesh->texcoordsSize(), mMesh->texcoords(), GL_STATIC_DRAW);
	mLoc_texcoord = mShaderProgram->bindVertexAttribute("texcoord", 2, 0, 0);
	glEnableVertexAttribArray(mLoc_texcoord);

	glBindVertexArray(0);
}

void Model::render() {
	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f, 8.f, 0.f));
	modelMatrix = glm::translate(modelMatrix, mMesh->center() * (-1.f));
	
	mShaderProgram->setUniformMatrix4f("TG", modelMatrix);
	mShaderProgram->setUniform4f("color", 1.f, 1.f, 1.f, 1.f);

	glEnable(GL_TEXTURE_2D);
	mTexture.use();
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mMesh->numVertices());
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_2D);
}