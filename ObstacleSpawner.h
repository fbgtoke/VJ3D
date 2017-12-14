#ifndef _OBSTACLE_SPAWNER_INCLUDE
#define _OBSTACLE_SPAWNER_INCLUDE

#include "utils.h"
#include "Obstacle.h"

class ObstacleSpawner : public Obstacle {
public:
  ObstacleSpawner();
  ~ObstacleSpawner() override;

  void init() override;
  void update(int deltaTime) override;
  void render() override;

  bool collides(const Model& m) const override;
  Obstacle::Type getType() const override;

  void setSpawnType(Obstacle::Type type);
  void setSpawnPeriod(float period);
  void setSpawnVel(float vel);

private:
  static const float kSpawnCoordinate;
  static const float kDespawnCoordinate;
  static bool outOfBounds(Obstacle* obstacle);

  Obstacle::Type mSpawnType;
  float mSpawnVel;
  float mSpawnPeriod;

  float mCurrentCycle;

  void spawnObstacle();
  std::list<Obstacle*> mSpawned;
};

#endif // _OBSTACLE_SPAWNER_INCLUDE