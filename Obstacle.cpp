#include "Obstacle.h"
#include "ObstacleTree.h"
#include "ObstacleCar.h"

Obstacle::Obstacle(ObstacleType type)
  : mType(type) {}

Obstacle::~Obstacle() {}

Obstacle::ObstacleType Obstacle::getType() const { return mType; }

Obstacle* Obstacle::create(ObstacleType type) {
  switch (type) {
    case TREE: return new ObstacleTree();
    case CAR: return new ObstacleCar();
    default: return nullptr;
  }
}

Obstacle* Obstacle::createFromStream(std::istringstream& sstream) {
  Obstacle* obstacle;

  unsigned int type;
  sstream >> type;

  switch (static_cast<ObstacleType>(type)) {
    case TREE: obstacle = readTree(sstream); break;
    case CAR: obstacle = readCar(sstream); break;
    default: obstacle = nullptr;
  }

  return obstacle;
}

Obstacle* Obstacle::readTree(std::istringstream& sstream) {
  unsigned int position;
  sstream >> position;

  ObstacleTree* obstacle = new ObstacleTree();
  obstacle->init(position);
  return obstacle;
}

Obstacle* Obstacle::readCar(std::istringstream& sstream) {
  float period, spd;
  sstream >> period >> spd;

  ObstacleCar* obstacle = new ObstacleCar();
  obstacle->init(period, spd);
  return obstacle;
}