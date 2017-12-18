#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "utils.h"
#include "Tilemap.h"
#include "Model.h"
#include "Player.h"
#include "Obstacle.h"
#include "ObstacleSpawner.h"

class Level {
public:
  Level();
  ~Level();

  void update(int deltaTime);
  void render();

  void setTilemap(Tilemap* tilemap);
  void setPlayer(Player* player);

  void addObstacle(Obstacle* obstacle);

  Tilemap* getTilemap();
  Player* getPlayer();

private:
  Tilemap* mTilemap;
  glm::ivec2 player2tilemap(const glm::vec3& position);

  Player* mPlayer;
  std::list<Obstacle*> mObstacles;
};

#endif // _LEVEL_INCLUDE