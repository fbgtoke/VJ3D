#include "Level.h"
#include "Game.h"

Level::Level()
  : mTilemap(nullptr), mPlayer(nullptr) {}

Level::~Level() {
  if (mTilemap != nullptr)
    delete mTilemap;

  if (mPlayer != nullptr)
    delete mPlayer;

  for (auto it = mObstacles.begin(); it != mObstacles.end(); ++it)
    if ((*it) != nullptr)
      delete (*it);

  mObstacles.clear();
}

void Level::update(int deltaTime) {
  playerUpdate(deltaTime);
  updateObstacles(deltaTime);
}

void Level::render() {
  if (mTilemap != nullptr)
    mTilemap->render();

  if (mPlayer != nullptr)
    mPlayer->render();

  for (Obstacle* obstacle : mObstacles)
    if (obstacle != nullptr)
      obstacle->render();
}

void Level::setTilemap(Tilemap* tilemap) {
  if (tilemap != nullptr) {
    if (mTilemap != nullptr)
      delete mTilemap;
    
    mTilemap = tilemap;
  }
}

void Level::setPlayer(Player* player) {
  if (player != nullptr) {
    if (mPlayer != nullptr)
      delete mPlayer;
    
    mPlayer = player;
  }
}

void Level::addObstacle(Obstacle* obstacle) {
  if (obstacle != nullptr)
    mObstacles.push_back(obstacle);
}

bool Level::obstacleAtTile(Obstacle::Type type, const glm::vec3& tile) {
  for (Obstacle* obstacle : mObstacles)
    if (obstacle->getPositionInTiles() == tile)
      return true;
  return false;
}

Tilemap* Level::getTilemap() { return mTilemap; }
Player* Level::getPlayer() { return mPlayer; }

void Level::playerUpdate(int deltaTime) {
  if (mPlayer == nullptr) return;

  playerInput();
  mPlayer->update(deltaTime);

  glm::vec3 position = mPlayer->getPositionInTiles();
  Tile::Type tile = mTilemap->getTile(glm::ivec2(position.x, position.z * (-1.f)));
  mPlayer->checkTile(tile);

  if (mPlayer->getState() == Player::Drowning && obstacleAtTile(Obstacle::Stone, position))
    mPlayer->changeState(Player::Idle);
}

void Level::playerInput() {
  if (!mPlayer->isIdle()) return;
  
  glm::vec3 direction(0.f);
  if (Game::instance().getKeyPressed('a'))
    direction = LEFT;
  else if (Game::instance().getKeyPressed('d'))
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

  if (jumping && !outOfBounds) {
    glm::vec3 min, max;
    if (obstacleAtTile(Obstacle::Stone, targetTile)) {
      Game::instance().getResource().mesh("stone.obj")->getMinMaxVertices(min, max);
      targetPosition.y += max.y;
    }

    mPlayer->moveTowards(targetPosition);
  }
}

void Level::updateObstacles(int deltaTime) {
  for (Obstacle* obstacle : mObstacles) {
    if (obstacle != nullptr) {
      obstacle->update(deltaTime);

      if (mPlayer->isAlive() && obstacle->collides(*mPlayer))
        mPlayer->checkCollision(obstacle);
    }
  }
}

glm::ivec2 Level::player2tilemap(const glm::vec3& position) {
  glm::ivec2 index;

  index.x = (int)position.x;
  index.y = (int)(position.z * -1.f);

  return index;
}