#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE

#include "utils.h"
#include "Tilemap.h"
#include "Model.h"
#include "Player.h"
#include "Obstacle.h"

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
  std::list<Model*> mModels;

  glm::ivec2 player2tilemap(const glm::vec3& position);

  Player* mPlayer;
  void playerUpdate(int deltaTime);
  void playerInput();
  void playerCheckTile();

  std::list<Obstacle*> mObstacles;
  void updateObstacles(int deltaTime);
};

#endif // _LEVEL_INCLUDE