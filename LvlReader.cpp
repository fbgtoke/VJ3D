#include "LvlReader.h"
#include "Game.h"

LvlReader::LvlReader() {}

LvlReader::~LvlReader() {}

void LvlReader::loadFromFile(const std::string& filename, std::list<Chunk*>& chunks) {
  std::ifstream stream(filename);
  if (!stream.is_open()) {
    std::cout << "Could not open LVL file " << filename << std::endl;
    return;
  }

  std::string line;
  while (getline(stream,line)) {
    if (line.find("chunk ") != std::string::npos) readChunk(line, chunks);
    else if (line.find("object ") != std::string::npos) readObject(line, chunks);
  }
}

void LvlReader::readChunk(const std::string& line, std::list<Chunk*>& chunks) {
  Chunk* chunk = new Chunk();

  std::string command;
  unsigned int type;
  unsigned int depth;

  std::istringstream sstream(line);
  sstream >> command >> type >> depth;

  chunk->init(static_cast<Chunk::ChunkType>(type), depth);

  chunks.push_back(chunk);
}

void LvlReader::readObject(const std::string& line, std::list<Chunk*>& chunks) {
  if (chunks.size() == 0) return;

  Chunk* chunk = chunks.back();
  unsigned int depth = chunk->getDepth();

  std::string command;

  std::istringstream sstream(line);
  sstream >> command;

  Obstacle* obstacle = Obstacle::createFromStream(sstream);

  if (obstacle != nullptr)
    chunk->addObstacle(obstacle);
  else
    std::cout << "Could not load obstacle from command: " << line << std::endl;
}