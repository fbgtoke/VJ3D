#include "Obstacle.h"
#include "ObstacleSpawner.h"
#include "Game.h"

Obstacle::Obstacle(Obstacle::Type type)
  : mType(type) {}

Obstacle::Obstacle::Type Obstacle::getType() const { return mType; }

void Obstacle::init() {
  Model::init();

  switch (mType) {
  case Obstacle::Cactus:
    setMesh(Game::instance().getResource().mesh("cactus.obj"));
    break;
  case Obstacle::Stump:
    setMesh(Game::instance().getResource().mesh("stump.obj"));
    break;
  case Obstacle::Stone:
    setMesh(Game::instance().getResource().mesh("stone.obj"));
    break;
  case Obstacle::Bonus:
    setMesh(Game::instance().getResource().mesh("bonus1.obj"));
    break;
  case Obstacle::Carriage:
    setMesh(Game::instance().getResource().mesh("carriage1.obj"));
    break;
  case Obstacle::Horse:
    setMesh(Game::instance().getResource().mesh("horse.obj"));
    break;
  case Obstacle::Boat:
    setMesh(Game::instance().getResource().mesh("boat.obj"));
    break;
  default:
    setMesh(Game::instance().getResource().mesh("cube.obj"));
    break;
  }

  setTexture(Game::instance().getResource().texture("palette.png"));
}

void Obstacle::update(int deltaTime) {
  Model::update(deltaTime);

  if (mVelocity.x >= 0.f)
    mScale.x = -1.f;
}