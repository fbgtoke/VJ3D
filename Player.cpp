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
    move(LEFT);
    setRotation(UP * (float)M_PI/2.f * 3.f);
  } else if (Game::instance().getKeyPressed('d')) {
    move(RIGHT);
    setRotation(UP * (float)M_PI/2.f * 1.f);
  } else if (Game::instance().getKeyPressed('w')) {
    move(IN);
    setRotation(UP * (float)M_PI/2.f * 2.f);
  } else if (Game::instance().getKeyPressed('s')) {
    move(OUT);
    setRotation(UP * (float)M_PI/2.f * 0.f);
  }
}

void Player::render() {
  Model::render();
}
