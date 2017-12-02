#ifndef _LVL_READER_INCLUDE
#define _LVL_READER_INCLUDE

#include "utils.h"
#include "Chunk.h"

class LvlReader {
public:
  LvlReader();
  ~LvlReader();

  void loadFromFile(const std::string& filename, std::list<Chunk*>& chunks);

private:
  void readChunk(const std::string& line, std::list<Chunk*>& chunks);
  void readObject(const std::string& line, std::list<Chunk*>& chunks);
};

#endif // _LVL_READER_INCLUDE