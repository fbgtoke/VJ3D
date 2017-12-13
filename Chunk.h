#ifndef _CHUNK_INCLUDE
#define _CHUNK_INCLUDE

#include "utils.h"
#include "Model.h"
#include "Obstacle.h"
#include "Player.h"

class Chunk {
public:
	Chunk();
	~Chunk();

  enum ChunkType {
    GRASS,
    ROAD,
    TRAIN,
    WATER,
    GOAL
  };

	void init(ChunkType type, unsigned int depth);
	void update(int deltaTime);
	void render();

  void setDepth(unsigned int depth);
  unsigned int getDepth() const;

  void setType(ChunkType type);
  ChunkType getType() const;

  void addObstacle(Obstacle* obstacle);

  const Obstacle* checkCollisions(const Player& player) const;
  bool hasObstacleAtPosition(Obstacle::ObstacleType type, unsigned int position) const;

private:
	unsigned int mDepth;
  ChunkType mType;

  void createFloor();
  void initFloor();
  std::list<Model*> mFloor;

	std::list<Obstacle*> mObstacles;
};

#endif // _CHUNK_INCLUDE