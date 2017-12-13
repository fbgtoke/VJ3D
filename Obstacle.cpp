#include "Obstacle.h"
#include "ObstacleTree.h"
#include "ObstacleCarSpawner.h"
#include "ObstacleLillypad.h"
#include "ObstacleBonus.h"

Obstacle::Obstacle(ObstacleType type)
  : mType(type) {}

Obstacle::ObstacleType Obstacle::getType() const { return mType; }

Obstacle* Obstacle::createFromStream(std::istringstream& sstream) {
  Obstacle* obstacle;

  unsigned int type;
  sstream >> type;

  switch (static_cast<ObstacleType>(type)) {
    case TREE:     obstacle = readTree(sstream); break;
    case CAR:      obstacle = readCar(sstream); break;
    case LILLYPAD: obstacle = readLillypad(sstream); break;
    case BONUS:    obstacle = readBonus(sstream); break;
    default:       obstacle = nullptr;
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
  float period, spd, var;
  sstream >> period >> spd >> var;

  ObstacleCarSpawner* obstacle = new ObstacleCarSpawner();
  obstacle->init(period, spd, var);
  return obstacle;
}

Obstacle* Obstacle::readLillypad(std::istringstream& sstream) {
  unsigned int position;
  sstream >> position;

  ObstacleLillypad* obstacle = new ObstacleLillypad();
  obstacle->init(position);
  return obstacle;
}

Obstacle* Obstacle::readBonus(std::istringstream& sstream) {
  unsigned int position;
  sstream >> position;

  ObstacleBonus* obstacle = new ObstacleBonus();
  obstacle->init(position);
  return obstacle;
}