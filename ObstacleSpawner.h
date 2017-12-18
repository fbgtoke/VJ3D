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

  bool collides(const Model* m) const override;

  void setSpawnType(Obstacle::Type type);
  void setSpawnPeriod(int period);
  void setSpawnVel(float vel);
  void setNumberOfTiles(unsigned int num);

  Obstacle::Type getSpawnType() const;

  Obstacle* getObstacleAtTile(const glm::vec3& tile);

private:
  static const float kBoundsMargin;
  bool outOfBounds(Obstacle* obstacle);

  Obstacle::Type mSpawnType;
  float mSpawnVel;
  int mSpawnPeriod;
  unsigned int mNumberOfTiles;

  int mCurrentCycle;

  void spawnObstacle();
  std::list<Obstacle*> mSpawned;
};

#endif // _OBSTACLE_SPAWNER_INCLUDE