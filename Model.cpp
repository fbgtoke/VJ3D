#include "Model.h"
#include "Game.h"

Model::Model() :
  mShaderProgram(nullptr),
  mTexture(nullptr), mMesh(nullptr) {}

Model::~Model() {}

void Model::init() {
	mPosition = glm::vec3(0.f);
	mShaderProgram = Game::instance().getResource().shader("simple");
}

void Model::render() {
  if (mMesh == nullptr) return;

	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, mPosition);
	modelMatrix = glm::translate(modelMatrix, mMesh->center() * (-1.f));
	
	mShaderProgram->setUniformMatrix4f("TG", modelMatrix);
	mShaderProgram->setUniform4f("color", 1.f, 1.f, 1.f, 1.f);

  if (mTexture != nullptr)
    mTexture->use();

  glEnable(GL_TEXTURE_2D);
	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, mMesh->numVertices());
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_2D);
}

void Model::setTexture(std::shared_ptr<Texture> texture) { mTexture = texture; }

void Model::setMesh(std::shared_ptr<Mesh> mesh) {
  mMesh = mesh;
  initVAO();
}

void Model::setPosition(const glm::vec3& position) { mPosition = position; }
glm::vec3 Model::getPosition() const { return mPosition; }
void Model::move(const glm::vec3& direction) { mPosition += direction; }

void Model::initVAO() {
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