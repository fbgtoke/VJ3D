#include "ObstacleLogSpawner.h"
#include "Game.h"

const float ObstacleLogSpawner::kSpawnCoordinate = 5 * (-1.f);
const float ObstacleLogSpawner::kDespawnCoordinate = TILES_PER_CHUNK + 5;

ObstacleLogSpawner::ObstacleLogSpawner()
  : Obstacle(Obstacle::LOG) {}

ObstacleLogSpawner::~ObstacleLogSpawner() {
  for (auto it = mLogs.begin(); it != mLogs.end(); ++it)
    delete (*it);
  mLogs.clear();
}

void ObstacleLogSpawner::init(float period, float spd, float var) {
  Obstacle::init();

  mPeriod = period;
  mSpeed = spd;
  mVariance = var;
  mCurrentPeriod = mPeriod + randomFloat(0.f, mVariance);

  setMesh(Game::instance().getResource().mesh("cube.obj"));
  setPositionInTiles(glm::vec3(0));
}

void ObstacleLogSpawner::update(int deltaTime) {
  Obstacle::update(deltaTime);

  mCurrentPeriod -= (float) deltaTime;
  if (mCurrentPeriod <= 0) {
    mCurrentPeriod += mPeriod + randomFloat(0.f, mVariance);
    addLog();
  }

  bool outOfBounds;
  auto it = mLogs.begin();
  while (it != mLogs.end()) {
    ObstacleLog* log = (*it);
    outOfBounds = 
      (log->getVelocity().x > 0.f && log->getPositionInTiles().x > kDespawnCoordinate) ||
      (log->getVelocity().x < 0.f && log->getPositionInTiles().x < kSpawnCoordinate);

    if (!outOfBounds) {
      log->update(deltaTime);
      ++it;
    } else {
      delete log;
      mLogs.erase(it++);
    }
  }    
}

void ObstacleLogSpawner::render() {
  //Obstacle::render();

  for (ObstacleLog* log : mLogs)
    log->render();
}

bool ObstacleLogSpawner::collides(const Model& m) const {
  for (ObstacleLog* log : mLogs)
    if (log->collides(m))
      return true;

  return false;
}

void ObstacleLogSpawner::addLog() {
  glm::vec3 spawn_position = getPositionInTiles();

  if (mSpeed > 0.f)
    spawn_position.x = kSpawnCoordinate;
  else
    spawn_position.x = kDespawnCoordinate;

  ObstacleLog* log = new ObstacleLog();
  log->init(mSpeed);
  log->setPositionInTiles(spawn_position);
  mLogs.push_back(log);
}