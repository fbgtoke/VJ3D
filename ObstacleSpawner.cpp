#include "ObstacleSpawner.h"

const float ObstacleSpawner::kBoundsMargin = 5;

ObstacleSpawner::ObstacleSpawner()
  : Obstacle(Obstacle::Spawner) {}

ObstacleSpawner::~ObstacleSpawner() {
  for (Obstacle* obstacle : mSpawned)
    delete obstacle;
  mSpawned.clear();
}

void ObstacleSpawner::init() {
  Obstacle::init();

  mSpawnType = Obstacle::Carriage;
  mSpawnPeriod = 0;
  mSpawnVel = 0.f;
  mCurrentCycle = 0;
  mNumberOfTiles = 0;
}

void ObstacleSpawner::update(int deltaTime) {
  Obstacle::update(deltaTime);

  mCurrentCycle += deltaTime;
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
void ObstacleSpawner::setSpawnPeriod(int period) {
  mSpawnPeriod = period;
  mCurrentCycle = 0;
}

void ObstacleSpawner::setSpawnVel(float vel) { mSpawnVel = vel; }
void ObstacleSpawner::setNumberOfTiles(unsigned int num) { mNumberOfTiles = num; }

bool ObstacleSpawner::outOfBounds(Obstacle* obstacle) {
  glm::vec3 pos = obstacle->getPositionInTiles();
  glm::vec3 vel = obstacle->getVelocity();

  return
    (vel.x > 0.f && pos.x > mNumberOfTiles + kBoundsMargin) ||
    (vel.x < 0.f && pos.x < 0 - kBoundsMargin);
}

void ObstacleSpawner::spawnObstacle() {
  glm::vec3 spawnPosition = getPositionInTiles();

  if (mSpawnVel > 0.f)
    spawnPosition.x = 0 - kBoundsMargin;
  else
    spawnPosition.x = mNumberOfTiles + kBoundsMargin;

  Obstacle* obstacle = new Obstacle(mSpawnType);
  obstacle->init();
  obstacle->setPositionInTiles(spawnPosition);
  obstacle->setVelocity(glm::vec3(mSpawnVel, 0.f, 0.f));
  mSpawned.push_back(obstacle);
}