#include "ObstacleSpawner.h"

const float ObstacleSpawner::kSpawnCoordinate = 5 * (-1.f);
const float ObstacleSpawner::kDespawnCoordinate = TILES_PER_CHUNK + 5;

ObstacleSpawner::ObstacleSpawner()
  : Obstacle(Obstacle::Spawner), mSpawnType(Obstacle::Carriage), mSpawnPeriod(0), mSpawnVel(0.f) {}

ObstacleSpawner::~ObstacleSpawner() {
  for (Obstacle* obstacle : mSpawned)
    delete obstacle;
  mSpawned.clear();
}

void ObstacleSpawner::init() {
  Obstacle::init();

  mCurrentCycle = 0.f;
}

void ObstacleSpawner::update(int deltaTime) {
  Obstacle::update(deltaTime);

  mCurrentCycle += (float)deltaTime;
  if (mSpawnPeriod > 0 && mCurrentCycle >= mSpawnPeriod) {
    spawnObstacle();
    mCurrentCycle -= mSpawnPeriod;
  }

  auto it = mSpawned.begin();
  while (it != mSpawned.end()) {
    Obstacle* obstacle = (*it);

    if (!outOfBounds(obstacle)) {
      obstacle->update(deltaTime);
      ++it;
    } else {
      delete obstacle;
      mSpawned.erase(it++);
    }
  }
}

void ObstacleSpawner::render() {
  //Obstacle::render();

  for (Obstacle* obstacle : mSpawned)
    obstacle->render();
}

bool ObstacleSpawner::collides(const Model& m) const {
  for (Obstacle* obstacle : mSpawned)
    if (obstacle->collides(m))
      return true;
  return false;
}

Obstacle::Type ObstacleSpawner::getType() const { return mSpawnType; }

void ObstacleSpawner::setSpawnType(Obstacle::Type type) { mSpawnType = type; }
void ObstacleSpawner::setSpawnPeriod(float period) { mSpawnPeriod = period; }
void ObstacleSpawner::setSpawnVel(float vel) { mSpawnVel = vel; }

bool ObstacleSpawner::outOfBounds(Obstacle* obstacle) {
  glm::vec3 pos = obstacle->getPositionInTiles();
  glm::vec3 vel = obstacle->getVelocity();

  return
    (vel.x > 0.f && pos.x > kDespawnCoordinate) ||
    (vel.x < 0.f && pos.x < kSpawnCoordinate);
}

void ObstacleSpawner::spawnObstacle() {
  glm::vec3 spawnPosition = getPositionInTiles();

  if (mSpawnVel > 0.f)
    spawnPosition.x = kSpawnCoordinate;
  else
    spawnPosition.x = kDespawnCoordinate;

  Obstacle* obstacle = new Obstacle(mSpawnType);
  obstacle->init();
  obstacle->setPositionInTiles(spawnPosition);
  obstacle->setVelocity(glm::vec3(mSpawnVel, 0.f, 0.f));
  mSpawned.push_back(obstacle);
}