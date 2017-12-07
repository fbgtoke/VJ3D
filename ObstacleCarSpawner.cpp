#include "ObstacleCarSpawner.h"
#include "Game.h"

const float ObstacleCarSpawner::kSpawnCoordinate = 3 * (-1.f);
const float ObstacleCarSpawner::kDespawnCoordinate = TILES_PER_CHUNK + 3;

ObstacleCarSpawner::ObstacleCarSpawner()
  : Obstacle(Obstacle::CAR) {}

ObstacleCarSpawner::~ObstacleCarSpawner() {
  for (ObstacleCar* car : mCars)
    if (car != nullptr)
      delete car;

  mCars.clear();
}

void ObstacleCarSpawner::init(float period, float spd, float var) {
  Obstacle::init();

  mPeriod = period;
  mSpeed = spd;
  mVariance = var;

  mCurrentPeriod = mPeriod + randomFloat(0.f, mVariance);

  setMesh(Game::instance().getResource().mesh("goal.obj"));
  setTexture(Game::instance().getResource().texture("goal.png"));

  setPositionInTiles(glm::vec3(0));
}

void ObstacleCarSpawner::update(int deltaTime) {
  Obstacle::update(deltaTime);

  mCurrentPeriod -= (float) deltaTime;
  if (mCurrentPeriod <= 0) {
    mCurrentPeriod += mPeriod + randomFloat(0.f, mVariance);
    addCar();
  }

  auto it = mCars.begin();
  while (it != mCars.end()) {
    ObstacleCar* car = (*it);
    if (car->getPositionInTiles().x <= kDespawnCoordinate) {
      car->update(deltaTime);
      ++it;
    } else {
      mCars.erase(it++);
    }
  }    
}

void ObstacleCarSpawner::render() {
  Obstacle::render();

  for (ObstacleCar* car : mCars)
    car->render();
}

void ObstacleCarSpawner::addCar() {
  glm::vec3 spawn_position = getPositionInTiles();
  spawn_position.x = kSpawnCoordinate;

  ObstacleCar* car = new ObstacleCar();
  car->init(mSpeed);
  car->setPositionInTiles(spawn_position);
  mCars.push_back(car);
}