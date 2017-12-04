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

  void setTexture(std::shared_ptr<Texture> texture);
  void setMesh(std::shared_ptr<Mesh> mesh);

  void setPosition(const glm::vec3& position);
  void setPositionInTiles(const glm::vec3& position);

  void setVelocity(const glm::vec3& velocity);

  void setRotation(const glm::vec3& rotation);
  void setRotationSpeed(const glm::vec3& speed);

  void move(const glm::vec3& direction);
  void moveInTiles(const glm::vec3& direction);

  glm::vec3 getPosition() const;
  glm::vec3 getPositionInTiles() const;
  glm::vec3 getCenter() const;

  glm::vec3 getVelocity() const;

  void setScale(float s);
  float getScale() const;

protected:
  void initVAO();
  GLuint mVAO;
  GLuint mVBO_vertices, mVBO_normals, mVBO_texcoord;
  GLuint mLoc_vertices, mLoc_normals, mLoc_texcoord;

  std::shared_ptr<ShaderProgram> mShaderProgram;

  std::shared_ptr<Texture> mTexture;
  std::shared_ptr<Mesh> mMesh;

  glm::vec3 mPosition;
  glm::vec3 mVelocity;

  glm::vec3 mCenter;
  glm::vec3 mSize;

  glm::vec3 mRotation;
  glm::vec3 mRotationSpeed;

  float mScale;
};

#endif // _MODEL_INCLUDE