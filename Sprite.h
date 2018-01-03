#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE

#include "utils.h"
#include "Texture.h"

class Sprite {
public:
  Sprite();
  virtual ~Sprite();

  static Sprite* create(const glm::vec2& size, Texture* texture,
    ShaderProgram* shader,
    const glm::vec4& rect = glm::vec4(0.f, 1.f, 0.f, 1.f));

  virtual void init();
  virtual void render();

  void resize(const glm::vec2& size);

  void setName(const std::string& name);
  std::string getName() const;

  void setTexture(Texture* texture,
    const glm::vec4& rect = glm::vec4(0.f, 1.f, 0.f, 1.f));
  void setTextureRect(const glm::vec4& rect);

  void setShader(ShaderProgram* shader);
  ShaderProgram* getShader();

  void setPosition(const glm::vec2& position);
  
  void move(const glm::vec2& direction);
  void move(float x, float y);

  void flipX();
  void flipY();

protected:
  std::string mName;
  glm::vec2 mPosition;
  glm::vec2 mSize;
  bool mFlipX;
  bool mFlipY;

  Texture* mTexture;
  ShaderProgram* mShader;

  std::vector<float> mVertices;
  std::vector<float> mTexcoords;

  GLuint mVAO;
  GLuint mVBO_vertices, mVBO_texcoords;
  GLuint mLoc_vertices, mLoc_texcoord;
  void free();
};

#endif // _SPRITE_INCLUDE