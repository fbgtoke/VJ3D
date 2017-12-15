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

  glm::vec3 position = mPlayer->getPositionInTiles();
  glm::ivec2 left  = player2tilemap(position + LEFT);
  glm::ivec2 right = player2tilemap(position + RIGHT);
  glm::ivec2 in    = player2tilemap(position + IN);
  glm::ivec2 out   = player2tilemap(position + OUT);

  if (Game::instance().getKeyPressed('a') && !mTilemap->outOfBounds(left))
    mPlayer->moveTowards(LEFT);
  else if (Game::instance().getKeyPressed('d') && !mTilemap->outOfBounds(right))
    mPlayer->moveTowards(RIGHT);
  else if (Game::instance().getKeyPressed('w') && !mTilemap->outOfBounds(in))
    mPlayer->moveTowards(IN);
  else if (Game::instance().getKeyPressed('s') && !mTilemap->outOfBounds(out))
    mPlayer->moveTowards(OUT);
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