#ifndef _CHUNK_INCLUDE
#define _CHUNK_INCLUDE

#include "utils.h"
#include "Model.h"

class Chunk {
public:
	Chunk();
	~Chunk();

	void init();
	void update(int deltaTime);
	void render();

  void setDepth(unsigned int depth);

private:
	unsigned int mDepth;

	std::list<Model*> mModels;
};

#endif // _CHUNK_INCLUDE