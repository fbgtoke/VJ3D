#ifndef _MODEL_INCLUDE
#define _MODEL_INCLUDE

#include "utils.h"
#include "Mesh.h"
#include "Texture.h"

class Model {
public:
  Model();
  virtual ~Model();

  virtual void init();
  virtual void update(int deltaTime);
  virtual void render();

  void setTexture(Texture* texture);
  void setMesh(Mesh* mesh);

  void setPosition(const glm::vec3& position);
  void setPositionInTiles(const glm::vec3& position);

  virtual void setVelocity(const glm::vec3& velocity);

  void setRotation(const glm::vec3& rotation);
  void setRotationSpeed(const glm::vec3& speed);

  void move(const glm::vec3& direction);
  void moveInTiles(const glm::vec3& direction);

  glm::vec3 getPosition() const;
  glm::vec3 getPositionInTiles() const;
  glm::vec3 getCenter() const;

  glm::vec3 getVelocity() const;

  void setScale(const glm::vec3& s);
  glm::vec3 getScale() const;

  glm::vec3 getSize() const;
  glm::vec3 getSizeInTiles() const;

  virtual bool collides(const Model& m) const;

  static Model* create(const std::string& mesh, const std::string& texture);

protected:
  ShaderProgram* mShaderProgram;

  Texture* mTexture;
  Mesh* mMesh;

  glm::vec3 mPosition;
  glm::vec3 mVelocity;

  glm::vec3 mCenter;
  glm::vec3 mSize;

  glm::vec3 mRotation;
  glm::vec3 mRotationSpeed;

  glm::vec3 mScale;
};

#endif // _MODEL_INCLUDE