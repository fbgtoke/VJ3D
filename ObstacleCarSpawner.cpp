#include "ObstacleCarSpawner.h"
#include "Game.h"

const float ObstacleCarSpawner::kSpawnCoordinate = 5 * (-1.f);
const float ObstacleCarSpawner::kDespawnCoordinate = TILES_PER_CHUNK + 5;

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

  bool outOfBounds;
  auto it = mCars.begin();
  while (it != mCars.end()) {
    ObstacleCar* car = (*it);
    outOfBounds = 
      (car->getVelocity().x > 0.f && car->getPositionInTiles().x > kDespawnCoordinate) ||
      (car->getVelocity().x < 0.f && car->getPositionInTiles().x < kSpawnCoordinate);

    if (!outOfBounds) {
      car->update(deltaTime);
      ++it;
    } else {
      mCars.erase(it++);
    }
  }    
}

void ObstacleCarSpawner::render() {
  //Obstacle::render();

  for (ObstacleCar* car : mCars)
    car->render();
}

bool ObstacleCarSpawner::collides(const Model& m) const {
  for (ObstacleCar* car : mCars)
    if (car->collides(m))
      return true;

  return false;
}

void ObstacleCarSpawner::addCar() {
  glm::vec3 spawn_position = getPositionInTiles();

  if (mSpeed > 0.f)
    spawn_position.x = kSpawnCoordinate;
  else
    spawn_position.x = kDespawnCoordinate;

  ObstacleCar* car = new ObstacleCar();
  car->init(mSpeed);
  car->setPositionInTiles(spawn_position);
  mCars.push_back(car);
}