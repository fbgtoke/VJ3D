#include "Player.h"
#include "Game.h"

Player::Player() {}
Player::~Player() {}

void Player::init() {
  Model::init();  
}

void Player::update(int deltaTime) {
  Model::update(deltaTime);

  if (Game::instance().getKeyPressed('a')) {
    moveInTiles(LEFT);
    setRotation(UP * (float)M_PI/2.f * 3.f);
  } else if (Game::instance().getKeyPressed('d')) {
    moveInTiles(RIGHT);
    setRotation(UP * (float)M_PI/2.f * 1.f);
  } else if (Game::instance().getKeyPressed('w')) {
    moveInTiles(IN);
    setRotation(UP * (float)M_PI/2.f * 2.f);
  } else if (Game::instance().getKeyPressed('s')) {
    moveInTiles(OUT);
    setRotation(UP * (float)M_PI/2.f * 0.f);
  }
}

void Player::render() {
  Model::render();
}
