#include "LevelGenerator.h"
#include "Game.h"

void LevelGenerator::generate(Level* level, const std::string& name) {
  level->init(name);

  LevelGenerator::generateTilemap(name, level);
  LevelGenerator::generateObstacles(name, level);
}

void LevelGenerator::generateTilemap(const std::string& name, Level* level) {
  Tilemap& tilemap = level->getTilemap();
  tilemap.loadFromFile("levels/" + name + "/tilemap.csv");
}

void LevelGenerator::generateObstacles(const std::string& name, Level* level) {
  CsvReader reader;
  reader.loadFromFile("levels/" + name + "/obstacles.csv");

  for (int i = 0; i < reader.getNumberOfRows(); ++i) {
    for (int j = 0; j < reader.getNumberOfColumns(); ++j) {
      glm::vec3 position;
      position.x = j;
      position.y = 0;
      position.z = (reader.getNumberOfRows() - 1 - i) * (-1);

      int value = reader.getCell(i, j);

      if (value != -1) {
        Obstacle* obstacle = LevelGenerator::generateObstacle(level, position, value);
        if (obstacle != nullptr) level->addObstacle(obstacle);
      }
    }
  }
}

Obstacle* LevelGenerator::generateObstacle(Level* level, const glm::vec3& position, int value) {
  Obstacle* obstacle;

  switch (value) {
  case 0:
    obstacle = new Obstacle(Obstacle::Cactus);
    obstacle->init();
    break;
  case 1:
    obstacle = new Obstacle(Obstacle::Stump);
    obstacle->init();
    break;
  case 2:
    obstacle = new Obstacle(Obstacle::Stone);
    obstacle->init();
    break;
  case 3:
    obstacle = new ObstacleBonus();
    obstacle->init();
    break;
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    obstacle = LevelGenerator::generateSpawner(level, value);
    break;
  case 16:
    obstacle = new Obstacle(Obstacle::Bottle);
    obstacle->init();
    break;
  default:
    obstacle = nullptr;
    break;
  }

  obstacle->setPositionInTiles(position);

  return obstacle;
}

Obstacle* LevelGenerator::generateSpawner(Level* level, int value) {
  Obstacle::Type type;
  int period;
  float vel;

  switch (value) {
  default:
  case 4:
    type = Obstacle::Carriage;
    period = Game::instance().getResource().Int("PeriodCarriageSlow");
    vel = Game::instance().getResource().Float("VelCarriageSlow") * (-1.f);
    break;
  case 5:
    type = Obstacle::Carriage;
    period = Game::instance().getResource().Int("PeriodCarriageFast");
    vel = Game::instance().getResource().Float("VelCarriageFast") * (-1.f);
    break;
  case 6:
    type = Obstacle::Carriage;
    period = Game::instance().getResource().Int("PeriodCarriageSlow");
    vel = Game::instance().getResource().Float("VelCarriageSlow");
    break;
  case 7:
    type = Obstacle::Carriage;
    period = Game::instance().getResource().Int("PeriodCarriageFast");
    vel = Game::instance().getResource().Float("VelCarriageFast");
    break;
  case 8:
    type = Obstacle::Horse;
    period = Game::instance().getResource().Int("PeriodHorseSlow");
    vel = Game::instance().getResource().Float("VelHorseSlow") * (-1.f);
    break;
  case 9:
    type = Obstacle::Horse;
    period = Game::instance().getResource().Int("PeriodHorseFast");
    vel = Game::instance().getResource().Float("VelHorseFast") * (-1.f);
    break;
  case 10:
    type = Obstacle::Horse;
    period = Game::instance().getResource().Int("PeriodHorseSlow");
    vel = Game::instance().getResource().Float("VelHorseSlow");
    break;
  case 11:
    type = Obstacle::Horse;
    period = Game::instance().getResource().Int("PeriodHorseFast");
    vel = Game::instance().getResource().Float("VelHorseFast");
    break;
  case 12:
    type = Obstacle::Boat;
    period = Game::instance().getResource().Int("PeriodBoatSlow");
    vel = Game::instance().getResource().Float("VelBoatSlow") * (-1.f);
    break;
  case 13:
    type = Obstacle::Boat;
    period = Game::instance().getResource().Int("PeriodBoatFast");
    vel = Game::instance().getResource().Float("VelBoatFast") * (-1.f);
    break;
  case 14:
    type = Obstacle::Boat;
    period = Game::instance().getResource().Int("PeriodBoatSlow");
    vel = Game::instance().getResource().Float("VelBoatSlow");
    break;
  case 15:
    type = Obstacle::Boat;
    period = Game::instance().getResource().Int("PeriodBoatFast");
    vel = Game::instance().getResource().Float("VelBoatFast");
    break;
  }

  ObstacleSpawner* spawner = new ObstacleSpawner();
  spawner->init();
  spawner->setSpawnType(type);
  spawner->setSpawnPeriod(period);
  spawner->setSpawnVel(vel);
  spawner->setNumberOfTiles(level->getTilemap().getWidth());

  return spawner;
}