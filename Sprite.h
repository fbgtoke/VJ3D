#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE

#include "utils.h"
#include "Model.h"
#include "Texture.h"
#include "ShaderProgram.h"

class Sprite {
public:
  Sprite();
  virtual ~Sprite();

  static Sprite* create(const glm::vec2& size, Texture* texture);

  virtual void init();
  virtual void update(int deltaTime);
  virtual void render();

  void resize(const glm::vec2& size);

  void setPosition(const glm::vec2& position);
  glm::vec2 getPosition() const;
  
  void move(const glm::vec2& direction);

  void setName(const std::string& name);
  std::string getName() const;

  void setShader(ShaderProgram* shader);
  ShaderProgram* getShader();

  void flipX();
  void flipY();

  void setTexture(Texture* texture);
  void setTextureRect(const glm::vec4& rect);

protected:
  std::string mName;

  ShaderProgram* mShader;
  Model mModel;

  int mCurrentTime;
};

#endif // _SPRITE_INCLUDE