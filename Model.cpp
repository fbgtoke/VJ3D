#include "Model.h"
#include "Game.h"

Model::Model() :
  mShaderProgram(nullptr),
  mTexture(nullptr), mMesh(nullptr) {}

Model::~Model() {}

void Model::init() {
	mPosition = glm::vec3(0.f);
  mVelocity = glm::vec3(0.f);
  mCenter = glm::vec3(0.f);
  mRotation = glm::vec3(0.f);
  mRotationSpeed = glm::vec3(0.f);
  mScale = glm::vec3(1.f);

	mShaderProgram = Game::instance().getResource().shader("simple");
}

void Model::update(int deltaTime) {
  move(mVelocity * (float)deltaTime);
  mRotation += mRotationSpeed * (float)deltaTime;
}

void Model::render() {
  if (mMesh == nullptr) return;

	glm::mat4 modelMatrix(1.f);
	modelMatrix = glm::translate(modelMatrix, mPosition);
  modelMatrix = glm::scale(modelMatrix, mScale);
  modelMatrix = glm::rotate(modelMatrix, mRotation.z, glm::vec3(0, 0, 1));
  modelMatrix = glm::rotate(modelMatrix, mRotation.y, glm::vec3(0, 1, 0));
  modelMatrix = glm::rotate(modelMatrix, mRotation.x, glm::vec3(1, 0, 0));
	modelMatrix = glm::translate(modelMatrix, mCenter * (-1.f));
	
	mShaderProgram->setUniformMatrix4f("TG", modelMatrix);
	mShaderProgram->setUniform4f("color", 1.f, 1.f, 1.f, 1.f);

  if (mTexture != nullptr)
    mTexture->use();

  glEnable(GL_TEXTURE_2D);
	glBindVertexArray(mMesh->getVAO());
	glDrawArrays(GL_TRIANGLES, 0, mMesh->numVertices());
	glBindVertexArray(0);
	glDisable(GL_TEXTURE_2D);
}

void Model::setTexture(Texture* texture) { mTexture = texture; }

void Model::setMesh(Mesh* mesh) {
  mMesh = mesh;
  mCenter = mMesh->center();
  mSize = mMesh->size();
}

void Model::setPosition(const glm::vec3& position) { mPosition = position; }
void Model::setPositionInTiles(const glm::vec3& position) {
  mPosition.x = (getSizeInTiles().x * 0.5f + position.x) * TILE_SIZE;
  mPosition.y = getSize().y * 0.5f + position.y * TILE_SIZE;
  mPosition.z = (getSizeInTiles().z * 0.5f + position.z) * TILE_SIZE;
}

void Model::setVelocity(const glm::vec3& velocity) { mVelocity = velocity; }
void Model::setRotation(const glm::vec3& rotation) { mRotation = rotation; }
void Model::setRotationSpeed(const glm::vec3& speed) { mRotationSpeed = speed; }

void Model::move(const glm::vec3& direction) { mPosition += direction; }
void Model::moveInTiles(const glm::vec3& direction) { mPosition += direction * TILE_SIZE; }

glm::vec3 Model::getPosition() const { return mPosition; }
glm::vec3 Model::getPositionInTiles() const {
  glm::vec3 position;
  return floor((mPosition - getSizeInTiles() * 0.5f)/TILE_SIZE);
}

glm::vec3 Model::getCenter() const { return mPosition + mCenter; }

glm::vec3 Model::getVelocity() const { return mVelocity; }

void Model::setScale(const glm::vec3& s) { mScale = s; }
glm::vec3 Model::getScale() const { return mScale; }

glm::vec3 Model::getSize() const { return mSize; }

glm::vec3 Model::getSizeInTiles() const {
  glm::vec3 size;
  size.x = ceil(mSize.x / TILE_SIZE);
  size.y = ceil(mSize.y / TILE_SIZE);
  size.z = ceil(mSize.z / TILE_SIZE);
  return size;
}

bool Model::collides(const Model& m) const {
  glm::vec3 mincoords1, mincoords2, maxcoords1, maxcoords2;
  mincoords1 = getPosition() - getSize() * 0.5f;
  mincoords2 = m.getPosition() - m.getSize() * 0.5f;
  maxcoords1 = mincoords1 + getSize();
  maxcoords2 = mincoords2 + m.getSize();

  return
    maxcoords1.x > mincoords2.x && mincoords1.x < maxcoords2.x &&
    maxcoords1.y > mincoords2.y && mincoords1.y < maxcoords2.y &&
    maxcoords1.z > mincoords2.z && mincoords1.z < maxcoords2.z;
}

Model* Model::create(const std::string& mesh, const std::string& texture) {
  Model* model = new Model();
  model->init();
  model->setMesh(Game::instance().getResource().mesh(mesh));
  model->setTexture(Game::instance().getResource().texture(texture));
  return model;
}