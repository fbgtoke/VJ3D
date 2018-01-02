#include "Level.h"
#include "Game.h"

Level::Level() {}

Level::~Level() {}

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

Tilemap& Level::getTilemap() { return mTilemap; }

glm::ivec2 Level::player2tilemap(const glm::vec3& position) {
  glm::ivec2 index;

  index.x = (int)position.x;
  index.y = (int)(position.z * -1.f);

  return index;
}