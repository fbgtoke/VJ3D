#ifndef _CHUNK_INCLUDE
#define _CHUNK_INCLUDE

#include "utils.h"
#include "Model.h"

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

  void addModel(Model* model);

private:
	unsigned int mDepth;
  ChunkType mType;

  void createFloor();
  void initFloor();
  std::list<Model*> mFloor;

	std::list<Model*> mModels;
};

#endif // _CHUNK_INCLUDE