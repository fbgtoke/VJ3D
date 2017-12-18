#include "Level.h"
#include "Game.h"

Level::Level()
  : mTilemap(nullptr), mPlayer(nullptr) {}

Level::~Level() {
  if (mTilemap != nullptr)
    delete mTilemap;
}

void Level::update(int deltaTime) {
  if (mPlayer->getState() != Player::Idle && mPlayer->getState() != Player::onBoat) return;
  
  glm::vec3 direction(0.f);
  if (Game::instance().getKeyPressed('a') && mPlayer->getState() != Player::onBoat)
    direction = LEFT;
  else if (Game::instance().getKeyPressed('d') && mPlayer->getState() != Player::onBoat)
    direction = RIGHT;
  else if (Game::instance().getKeyPressed('w'))
    direction = IN;
  else if (Game::instance().getKeyPressed('s'))
    direction = OUT;

  glm::vec3 targetTile = mPlayer->getPositionInTiles() + direction;
  glm::vec3 targetPosition = mPlayer->getPosition() + direction * TILE_SIZE;
  targetPosition.y = mPlayer->getSize().y * 0.5f;

  bool jumping = (direction != glm::vec3(0.f));
  bool outOfBounds = mTilemap->outOfBounds(player2tilemap(targetTile));

  if (jumping && !outOfBounds)
    mPlayer->moveTowards(targetPosition);
}

void Level::render() {}

void Level::setTilemap(Tilemap* tilemap) {
  if (tilemap != nullptr) {
    mTilemap = tilemap;
  }
}

void Level::setPlayer(Player* player) {
  if (player != nullptr) {
    if (mPlayer != nullptr) {
      Game::instance().getScene()->removeModel(mPlayer);
      delete mPlayer;
    }

    mPlayer = player;
    Game::instance().getScene()->addModel(player);
  }
}

void Level::addObstacle(Obstacle* obstacle) {
  if (obstacle != nullptr) {
    mObstacles.push_back(obstacle);
    Game::instance().getScene()->addModel(obstacle);
  }
}

Tilemap* Level::getTilemap() { return mTilemap; }
Player* Level::getPlayer() { return mPlayer; }

glm::ivec2 Level::player2tilemap(const glm::vec3& position) {
  glm::ivec2 index;

  index.x = (int)position.x;
  index.y = (int)(position.z * -1.f);

  return index;
}