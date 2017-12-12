#ifndef _LVL_READER_INCLUDE
#define _LVL_READER_INCLUDE

#include "utils.h"
#include "Chunk.h"
#include "Obstacle.h"

class LvlReader {
public:
  static void loadFromFile(const std::string& filename, std::list<Chunk*>& chunks);

private:
  static void readChunk(const std::string& line, std::list<Chunk*>& chunks);
  static void readObject(const std::string& line, std::list<Chunk*>& chunks);

  Obstacle* readTree(std::istringstream& sstream);
  Obstacle* readCar(std::istringstream& sstream);
};

#endif // _LVL_READER_INCLUDE