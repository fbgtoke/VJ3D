#include "Sprite.h"
#include "Game.h"

Sprite::Sprite()
  : mShader(nullptr) {}

Sprite::~Sprite() {}

Sprite* Sprite::create(const glm::vec2& size, Texture* texture) {
  Sprite* sprite = new Sprite();
  sprite->init();
  sprite->resize(size);
  sprite->setTexture(texture);
  return sprite;
}

void Sprite::init() {
  mName = "";
  setShader(Game::instance().getResource().shader("sprite"));

  mModel.init();
  mModel.setMesh(Game::instance().getResource().mesh("plane.obj"));

  mCurrentTime = 0;
}

void Sprite::update(int deltaTime) {
  mCurrentTime += deltaTime;
}

void Sprite::render() {
  mShader->setUniform2f("resolution", SCREEN_WIDTH, SCREEN_HEIGHT);
  mShader->setUniform1i("time", mCurrentTime);

  glm::vec3 offset;
  offset.x = abs(mModel.getScale().x) * 0.5f;
  offset.y = abs(mModel.getScale().y) * 0.5f;
  offset.z = 0.f;

  mModel.move(offset);
  mModel.render();
  mModel.move(-offset);
}

void Sprite::resize(const glm::vec2& size) {
  mModel.setScale(glm::vec3(size.x, size.y, 1.f));
}

void Sprite::setPosition(const glm::vec2& position) {
  glm::vec3 p(position.x, position.y, 0.f);
  mModel.setPosition(p);
}

glm::vec2 Sprite::getPosition() const {
  return glm::vec2(mModel.getPosition().x, mModel.getPosition().y);
}

void Sprite::move(const glm::vec2& direction) {
  mModel.move(glm::vec3(direction.x, direction.y, 0.f));
}

void Sprite::setName(const std::string& name) { mName = name; }
std::string Sprite::getName() const { return mName; }

void Sprite::setShader(ShaderProgram* shader) { mShader = shader; }
ShaderProgram* Sprite::getShader() { return mShader; }

void Sprite::flipX() {
  glm::vec3 scale = mModel.getScale();
  scale.x *= -1.f;
  mModel.setScale(scale);
}

void Sprite::flipY() {
  glm::vec3 scale = mModel.getScale();
  scale.y *= -1.f;
  mModel.setScale(scale);
}

void Sprite::setTexture(Texture* texture) { mModel.setTexture(texture); }

void Sprite::setTextureRect(const glm::vec4& rect) {
  float* texcoords = (float*) malloc(12 * sizeof(float));
     texcoords[0] = rect[0];  texcoords[1] = rect[2];
     texcoords[2] = rect[0];  texcoords[3] = rect[3];
     texcoords[4] = rect[1];  texcoords[5] = rect[2];
     texcoords[6] = rect[1];  texcoords[7] = rect[2];
     texcoords[8] = rect[0];  texcoords[9] = rect[3];
    texcoords[10] = rect[1]; texcoords[11] = rect[3];
  mModel.getMesh()->setTexCoords(texcoords, 12);
}