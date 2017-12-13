#ifndef _OBSTACLE_LOG_SPAWNER_INCLUDE
#define _OBSTACLE_LOG_SPAWNER_INCLUDE

#include "Obstacle.h"
#include "ObstacleLog.h"

class ObstacleLogSpawner : public Obstacle {
public:
  ObstacleLogSpawner();
  ~ObstacleLogSpawner() override;

  void init(float period, float spd, float var);
  void update(int deltaTime) override;
  void render() override;

  bool collides(const Model& m) const override;

private:
  static const float kSpawnCoordinate;
  static const float kDespawnCoordinate;

  float mPeriod;
  float mSpeed;
  float mVariance;

  float mCurrentPeriod;

  void addLog();
  std::list<ObstacleLog*> mLogs;
};

#endif // _OBSTACLE_LOG_SPAWNER_INCLUDE