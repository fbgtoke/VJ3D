#ifndef _LEVEL_INFO_INCLUDE
#define _LEVEL_INFO_INCLUDE

#include "utils.h"
#include "Texture.h"

class LevelInfo {
public:
  LevelInfo();
  ~LevelInfo();

  void setName(const std::string& name);
  std::string getName() const;

  void readThumbnail();
  Texture* getThumbnail();

  void readHighscores();
  void saveHighscores() const;
  std::vector<unsigned int> getHighscores() const;

private:
  std::string mName;
  Texture* mThumbnail;
  std::vector<unsigned int> mHighscores;
};

#endif // _LEVEL_INFO_INCLUDE