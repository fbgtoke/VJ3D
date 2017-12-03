#include "Model.h"
#include "Game.h"

Model::Model() :
  mShaderProgram(nullptr),
  mTexture(nullptr), mMesh(nullptr) {}

Model::~Model() {}

void Model::init() {
	mPositionInTiles = glm::vec3(0.f);
  mCenter = glm::vec3(0.f);
  mSizeInTiles = glm::vec3(0.f);
  mRotation = glm::vec3(0.f);
  mRotationSpeed = glm::vec3(0.f);
  mScale = 1.f;

	mShaderProgram = Game::instance().getResource().shader("simple");
}

void Model::update(int deltaTime) {
  mRotation += mRotationSpeed * (float)deltaTime;
}

void Model::render() {
  if (mMesh == nullptr) return;

	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, mPositionInTiles * TILE_SIZE);
  modelMatrix = glm::translate(modelMatrix, glm::vec3(TILE_SIZE) * 0.5f);
  modelMatrix = glm::translate(modelMatrix, mSize * 0.5f);
  modelMatrix = glm::rotate(modelMatrix, mRotation.z, glm::vec3(0, 0, 1));
  modelMatrix = glm::rotate(modelMatrix, mRotation.y, glm::vec3(0, 1, 0));
  modelMatrix = glm::rotate(modelMatrix, mRotation.x, glm::vec3(1, 0, 0));
  modelMatrix = glm::scale(modelMatrix, glm::vec3(mScale));
	modelMatrix = glm::translate(modelMatrix, mCenter * (-1.f));
	
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

  mCenter = mMesh->center();
  mSizeInTiles = mMesh->sizeInTiles();
  mSize = mMesh->size();
}

void Model::setPositionInTiles(const glm::vec3& position) { mPositionInTiles = position; }
void Model::setRotation(const glm::vec3& rotation) { mRotation = rotation; }
void Model::setRotationSpeed(const glm::vec3& speed) { mRotationSpeed = speed; }

void Model::move(const glm::vec3& direction) { mPositionInTiles += direction; }

glm::vec3 Model::getPositionInTiles() const { return mPositionInTiles; }
glm::vec3 Model::getCenter() const { return mPositionInTiles*TILE_SIZE + mCenter; }

void Model::setScale(float s) { mScale = s; }
float Model::getScale() const { return mScale; }

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