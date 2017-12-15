#include "Obstacle.h"
#include "ObstacleSpawner.h"
#include "Game.h"

Obstacle::Obstacle(Obstacle::Type type)
  : mType(type) {}

Obstacle::Obstacle::Type Obstacle::getType() const { return mType; }

void Obstacle::init() {
  ModelAnimated::init();

  switch (mType) {
  case Obstacle::Cactus:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("cactus.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Stump:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("stump.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Stone:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("stone.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Carriage:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("carriage1.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationcarriage1.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("carriage2.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationcarriage2.obj"));
    mAnimation.setTimePerFrame(100);
    mAnimation.changeAnimation(0);
    break;
  case Obstacle::Horse:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("horse.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationhorse1.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("horse.obj"));
    mAnimation.addFrame(0, Game::instance().getResource().mesh("animationhorse2.obj"));
    mAnimation.setTimePerFrame(100);
    mAnimation.changeAnimation(0);
    setRotation(glm::vec3(0.f, (float)M_PI/2.f, 0.f));
    break;
  case Obstacle::Boat:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("boat.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  default:
    mAnimation.setNumberOfAnimations(1);
    mAnimation.addFrame(0, Game::instance().getResource().mesh("cube.obj"));
    mAnimation.setTimePerFrame(0);
    mAnimation.changeAnimation(0);
    break;
  }

  setMesh(mAnimation.getCurrentFrame());
  setTexture(Game::instance().getResource().texture("palette.png"));
}

void Obstacle::update(int deltaTime) {
  ModelAnimated::update(deltaTime);

  if (mVelocity.x >= 0.f)
    mScale.x = -1.f;
}

void Obstacle::render() {
  ModelAnimated::render();

  if (mType != Obstacle::Stone && mType != Obstacle::Boat)
    renderShadow();
}

void Obstacle::renderShadow() {
  Model shadow;
  shadow.init();
  shadow.setMesh(Game::instance().getResource().mesh("plane.obj"));
  shadow.setTexture(Game::instance().getResource().texture("shadow.png"));

  glm::vec3 position = getPosition();
  position.y = 0.001f;
  position.x += TILE_SIZE * 0.125f;
  shadow.setPosition(position);

  const float base = getCenter().y - getSize().y * 0.5f;
  const float max_height = TILE_SIZE * 2.f;
  const float min_scale = std::max(0.f, 1.f - base / max_height);
  glm::vec3 scale;
  scale.x = min_scale * (getSizeInTiles().x + getSizeInTiles().y) * 0.75f;
  scale.y = 1.f;
  scale.z = min_scale * getSizeInTiles().z * 1.5f;
  shadow.setScale(scale);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthMask(false);
  shadow.render();
  glDepthMask(true);
  glDisable(GL_BLEND);
}