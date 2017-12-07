#ifndef _OBSTACLE_CAR_SPAWNER_INCLUDE
#define _OBSTACLE_CAR_SPAWNER_INCLUDE

#include "Obstacle.h"
#include "ObstacleCar.h"

class ObstacleCarSpawner : public Obstacle {
public:
  ObstacleCarSpawner();
  ~ObstacleCarSpawner() override;

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

  void addCar();
  std::list<ObstacleCar*> mCars;
};

#endif // _OBSTACLE_CAR_SPAWNER_INCLUDE