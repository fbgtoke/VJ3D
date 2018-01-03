#include "Level.h"
#include "Game.h"

Level::Level() {}

Level::~Level() {}

void Level::update(int deltaTime) {
  auto it = mObstacles.begin();
  while (it != mObstacles.end()) {
    Obstacle* obstacle = (*it);

    if (obstacle->hasBeenDestroyed()) {
      if (obstacle->getType() == Obstacle::Bonus) {
        ObstacleBonus* bonus = dynamic_cast<ObstacleBonus*>(obstacle);

        if (bonus->picked()) {
          int score = Game::instance().getResource().Int("score");
          int scorePerBonus = Game::instance().getResource().Int("scorePerBonus");
          Game::instance().getResource().setInt("score", score + scorePerBonus);
        }
      }

      delete obstacle;
      mObstacles.erase(it++);
    } else {
      obstacle->update(deltaTime);
      it++;
    }
  }
}

void Level::render() {
  mTilemap.render();

  for (Obstacle* obstacle : mObstacles)
    obstacle->render();
}

void Level::checkCollisions(Model* model) {
  for (Obstacle* obstacle : mObstacles) {
    if (obstacle->getType() == Obstacle::Spawner) {
      std::list<Obstacle*> spawnedList = (dynamic_cast<ObstacleSpawner*>(obstacle))->getSpawned();
      for (Obstacle* spawned : spawnedList) {
        if (model != spawned && model->collides(spawned))
          model->onCollision(spawned);
      }
    } else {
      if (model != obstacle && model->collides(obstacle))
        model->onCollision(obstacle);
    }
  }
}

void Level::addObstacle(Obstacle* obstacle) {
  if (obstacle != nullptr) {
    mObstacles.push_back(obstacle);
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