#include "Sprite.h"
#include "Game.h"

Sprite::Sprite()
  : mTexture(nullptr), mShader(nullptr) {}

Sprite::~Sprite() {
  free();
}

Sprite* Sprite::create(const glm::vec2& size, Texture* texture,
  ShaderProgram* shader, const glm::vec4& rect) {

  Sprite* sprite = new Sprite();
  sprite->init();
  sprite->resize(size);
  sprite->setTexture(texture, rect);
  sprite->setShader(shader);
  return sprite;
}

void Sprite::init() {
  mPosition = glm::vec2(0.f);
  mSize = glm::vec2(1.f);
  mFlipX = false;
  mFlipY = false;

  mVertices = {
    -0.5f,  0.5f, 0.f,
    -0.5f, -0.5f, 0.f,
     0.5f,  0.5f, 0.f,
     0.5f, -0.5f, 0.f,
     0.5f,  0.5f, 0.f,
    -0.5f, -0.5f, 0.f
  };
  setTextureRect(glm::vec4(0.f, 1.f, 0.f, 1.f));

  mVAO = GL_INVALID_VALUE;
  mVBO_vertices = mVBO_texcoords = GL_INVALID_VALUE;
}

void Sprite::render() {
  if (mTexture != nullptr && mShader != nullptr) {
    free();

    glm::mat4 TG = glm::mat4(1.f);
    TG = glm::translate(TG, glm::vec3(mPosition, 0.f));
    TG = glm::translate(TG, glm::vec3(mSize, 0.f) * 0.5f);
    TG = glm::scale(TG, glm::vec3(mSize, 0.f));

    if (mFlipX)
      TG = glm::scale(TG, glm::vec3(-1.f, 1.f, 1.f));
    if (mFlipY)
      TG = glm::scale(TG, glm::vec3(1.f, -1.f, 1.f));

    mShader->setUniformMatrix4f("TG", TG);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    glGenBuffers(1, &mVBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), &mVertices[0], GL_STATIC_DRAW);
    mLoc_vertices = mShader->bindVertexAttribute("vertex", 3, 0, 0);
    glEnableVertexAttribArray(mLoc_vertices);

    glGenBuffers(1, &mVBO_texcoords);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO_texcoords);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &mTexcoords[0], GL_STATIC_DRAW);
    mLoc_texcoord = mShader->bindVertexAttribute("texcoord", 2, 0, 0);
    glEnableVertexAttribArray(mLoc_texcoord);

    mShader->setUniform1i("tex", 0);
    mTexture->use();

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }
}

void Sprite::resize(const glm::vec2& size) { mSize = size; }

void Sprite::setTexture(Texture* texture, const glm::vec4& rect) {
  mTexture = texture;
  setTextureRect(rect);
}

void Sprite::setTextureRect(const glm::vec4& rect) {
  mTexcoords = {
    rect[0], rect[3],
    rect[0], rect[2],
    rect[1], rect[3],
    rect[1], rect[2],
    rect[1], rect[3],
    rect[0], rect[2]
  };
}

void Sprite::setShader(ShaderProgram* shader) { mShader = shader; }
ShaderProgram* Sprite::getShader() { return mShader; }

void Sprite::setPosition(const glm::vec2& position) { mPosition = position; }

void Sprite::move(const glm::vec2& direction) { mPosition += direction; }
void Sprite::move(float x, float y) {
  mPosition.x += x;
  mPosition.y += y;
}

void Sprite::flipX() { mFlipX = !mFlipX; }
void Sprite::flipY() { mFlipY = !mFlipY; }

void Sprite::free() {
  if (mVBO_vertices != GL_INVALID_VALUE) {
    glDeleteBuffers(1, &mVBO_vertices);
    mVBO_vertices = GL_INVALID_VALUE;
  }

  if (mVBO_texcoords != GL_INVALID_VALUE) {
    glDeleteBuffers(1, &mVBO_texcoords);
    mVBO_texcoords = GL_INVALID_VALUE;
  }

  if (mVAO != GL_INVALID_VALUE) {
    glDeleteVertexArrays(1, &mVAO);
    mVAO = GL_INVALID_VALUE;
  }
}