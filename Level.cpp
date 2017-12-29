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

  /* Check for boat */
  glm::vec3 adjacent[3][3];
  mPlayer->getAdjacentTiles(adjacent);
  Obstacle* boat = nullptr;
  if (direction == IN) {
    boat = getObstacleAtTile(adjacent[0][0]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = getObstacleAtTile(adjacent[0][1]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = getObstacleAtTile(adjacent[0][2]);
  } else if (direction == OUT) {
    boat = getObstacleAtTile(adjacent[2][0]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = getObstacleAtTile(adjacent[2][1]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = getObstacleAtTile(adjacent[2][2]);
  }

  /* Check for stone */
  Obstacle* stone = nullptr;
  if (direction == IN)
    stone = getObstacleAtTile(adjacent[0][1]);
  else if (direction == OUT)
    stone = getObstacleAtTile(adjacent[0][2]);

  glm::vec3 targetTile = mPlayer->getPositionInTiles() + direction;
  bool jumping = (direction != glm::vec3(0.f));
  bool outOfBounds = mTilemap->outOfBounds(player2tilemap(targetTile));

  glm::vec3 targetPosition;
  if (jumping && !outOfBounds) {
    if (boat != nullptr && boat->getType() == Obstacle::Boat)  {
      mPlayer->moveTowardsBoat(boat);
    } else if (stone != nullptr && stone->getType() == Obstacle::Stone) {
      targetPosition = stone->getTopCenter();
      targetPosition.y += mPlayer->getSize().y * 0.5f;
      mPlayer->moveTowards(targetPosition);
    } else {
      targetPosition = mPlayer->getPosition() + direction * TILE_SIZE;
      targetPosition.y = mPlayer->getSize().y * 0.5f;
      mPlayer->moveTowards(targetPosition);
    }
  }

  /* Check standing tile */
  if (mPlayer->getState() == Player::Idle) {
    glm::vec3 standingTile = mPlayer->getPositionInTiles();
    Tile::Type tile = mTilemap->getTile(player2tilemap(standingTile));

    stone = getObstacleAtTile(standingTile);

    if (tile == Tile::Water && !obstacleOfTypeAtTile(Obstacle::Stone, standingTile))
      mPlayer->changeState(Player::Drowning);
    else if (tile == Tile::Goal)
      Game::instance().changeScene(Scene::SCENE_WIN);
  }
}

void Level::setTilemap(Tilemap* tilemap) {
  if (tilemap != nullptr) {
    mTilemap = tilemap;
  }
}

void Level::setPlayer(Player* player) {
  if (player != nullptr) {
    if (mPlayer != nullptr)
      Game::instance().getScene()->removeModel(mPlayer);

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

void Level::removeObstacle(Obstacle* obstacle) {
  if (obstacle != nullptr) {
    auto it = mObstacles.begin();
    while (it != mObstacles.end() && (*it) != obstacle) ++it;
    if ((*it) == obstacle) 
      mObstacles.erase(it);
  }
}

Obstacle* Level::getObstacleAtTile(const glm::vec3& tile) {
  for (Obstacle* obstacle : mObstacles) {
    if (obstacle->getType() != Obstacle::Spawner) {
      if (obstacle->getPositionInTiles() == tile)
        return obstacle;
    } else {
      ObstacleSpawner* spawner = dynamic_cast<ObstacleSpawner*> (obstacle);
      Obstacle* spawned = spawner->getObstacleAtTile(tile);
      if (spawned != nullptr)
        return spawned;
    }
  }

  return nullptr;
}

bool Level::obstacleOfTypeAtTile(Obstacle::Type type, const glm::vec3& tile) {
  Obstacle* obstacle = getObstacleAtTile(tile);
  return obstacle != nullptr && obstacle->getType() == type;
}

Tilemap* Level::getTilemap() { return mTilemap; }
Player* Level::getPlayer() { return mPlayer; }

glm::ivec2 Level::player2tilemap(const glm::vec3& position) {
  glm::ivec2 index;

  index.x = (int)position.x;
  index.y = (int)(position.z * -1.f);

  return index;
}