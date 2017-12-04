#include "ObstacleCarSpawner.h"

const float ObstacleCarSpawner::kSpawnCoordinate = 3 * TILE_SIZE * (-1.f);
const float ObstacleCarSpawner::kDespawnCoordinate = CHUNK_SIZE + 3 * TILE_SIZE;

ObstacleCarSpawner::ObstacleCarSpawner()
  : Obstacle(Obstacle::CAR) {}

ObstacleCarSpawner::~ObstacleCarSpawner() {
  for (ObstacleCar* car : mCars)
    if (car != nullptr)
      delete car;

  mCars.clear();
}

void ObstacleCarSpawner::init(float period, float spd) {
  mPeriod = period;
  mSpeed = spd;

  mCurrentPeriod = 0.f;
}

void ObstacleCarSpawner::update(int deltaTime) {
  Obstacle::update(deltaTime);

  mCurrentPeriod += (float) deltaTime;
  if (mCurrentPeriod >= mPeriod) {
    mCurrentPeriod -= mPeriod;
    addCar();
  }

  auto it = mCars.begin();
  while (it != mCars.end()) {
    ObstacleCar* car = (*it);
    if (car->getPosition().x <= kDespawnCoordinate) {
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
  glm::vec3 spawn_position = mPosition;
  spawn_position.x = kSpawnCoordinate;

  ObstacleCar* car = new ObstacleCar();
  car->init(mSpeed);
  car->setPosition(spawn_position);
  mCars.push_back(car);
}