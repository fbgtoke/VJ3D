#include "Player.h"
#include "Game.h"

Player::Player() {}
Player::~Player() {}

void Player::init() {
  Model::init();

  mFrames[0] = Game::instance().getResource().mesh("cowboy.obj");
  mFrames[1] = Game::instance().getResource().mesh("animationcowboy.obj");

  setMesh(mFrames[0]);
  setTexture(Game::instance().getResource().texture("cowboy.png"));
}

void Player::update(int deltaTime) {
  Model::update(deltaTime);

  static int frame = 0;
  static int count = 0;

  if (count >= 10) {
    setMesh(mFrames[frame]);
    frame = not(frame);
    count = 0;
  }
  count++;

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
