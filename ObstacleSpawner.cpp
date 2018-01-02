#include "ObstacleSpawner.h"
#include "Game.h"

ObstacleSpawner::ObstacleSpawner()
  : Obstacle(Obstacle::Spawner) {}

ObstacleSpawner::~ObstacleSpawner() {}

void ObstacleSpawner::init() {
  Obstacle::init();

  kBoundsMargin = Game::instance().getResource().Float("BoundsMargin");
  kPeriodVariability = Game::instance().getResource().Int("PeriodVariability");

  mSpawnType = Obstacle::Carriage;
  mSpawnPeriod = 0;
  mSpawnVel = 0.f;
  mCurrentCycle = 0;
  mNumberOfTiles = 0;

  mEnableRendering = false;
}

void ObstacleSpawner::update(int deltaTime) {
  Obstacle::update(deltaTime);

  mCurrentCycle -= deltaTime;
  if (mSpawnPeriod > 0 && mCurrentCycle <= 0) {
    spawnObstacle();
    mCurrentCycle = mSpawnPeriod + randomInt(0, kPeriodVariability);
  }

  auto it = mSpawned.begin();
  while (it != mSpawned.end()) {
    Obstacle* obstacle = (*it);

    if (!outOfBounds(obstacle)) {
      obstacle->update(deltaTime);
      ++it;
    } else {
      obstacle->destroy();
      delete obstacle;
      mSpawned.erase(it++);
    }
  }
}

void ObstacleSpawner::afterRender() {
  Obstacle::afterRender();
  
  for (Obstacle* obstacle : mSpawned)
    obstacle->render();
}

bool ObstacleSpawner::collides(const Model* m) const { return false; }

void ObstacleSpawner::setSpawnType(Obstacle::Type type) { mSpawnType = type; }
void ObstacleSpawner::setSpawnPeriod(int period) {
  mSpawnPeriod = period;
  mCurrentCycle = randomInt(0, kPeriodVariability);
}

void ObstacleSpawner::setSpawnVel(float vel) { mSpawnVel = vel; }
void ObstacleSpawner::setNumberOfTiles(unsigned int num) { mNumberOfTiles = num; }

Obstacle::Type ObstacleSpawner::getSpawnType() const { return mSpawnType; }

Obstacle* ObstacleSpawner::getObstacleAtTile(const glm::vec3& tile) {
  for (Obstacle* obstacle : mSpawned)
    if (obstacle->getPositionInTiles() == tile)
      return obstacle;
  return nullptr;
}

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