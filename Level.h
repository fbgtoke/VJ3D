#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "utils.h"
#include "LevelInfo.h"
#include "Tilemap.h"
#include "Model.h"
#include "Obstacle.h"
#include "ObstacleSpawner.h"
#include "ObstacleBonus.h"

class Level {
public:
  Level();
  ~Level();

  void loadFromFile(const std::string& filename);

  void init(const std::string& name);
  void update(int deltaTime);
  void render();

  void checkCollisions(Model* model);

  void addObstacle(Obstacle* obstacle);
  void removeObstacle(Obstacle* obstacle);
  
  Obstacle* getObstacleAtTile(const glm::vec3& tile);
  bool obstacleOfTypeAtTile(Obstacle::Type type, const glm::vec3& tile);

  Tilemap& getTilemap();
  glm::ivec2 player2tilemap(const glm::vec3& position);

  LevelInfo getLevelInfo() const;

private:
  LevelInfo mLevelInfo;
  Tilemap mTilemap;
  std::list<Obstacle*> mObstacles;
};

#endif // _LEVEL_INCLUDE