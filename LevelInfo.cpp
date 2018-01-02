#include "LevelInfo.h"
#include "Game.h"

LevelInfo::LevelInfo()
  : mThumbnail(nullptr) {}

LevelInfo::~LevelInfo() {}

void LevelInfo::setName(const std::string& name) {
  mName = name;

  readThumbnail();
  readHighscores();
}

std::string LevelInfo::getName() const { return mName; }

void LevelInfo::readThumbnail() {
  mThumbnail =
    Game::instance().getResource().texture("levels/" + mName + "/thumbnail.png", true);
}

Texture* LevelInfo::getThumbnail() { return mThumbnail; }

void LevelInfo::readHighscores() {
  std::ifstream stream("levels/" + mName + "/highscores.txt");

  if (stream.is_open()) {
    unsigned int score;
    while (stream >> score) {
      mHighscores.push_back(score);
    }
    stream.close();
  } else {
    std::cout << "Could not find highscores for level " << mName << std::endl;
    mHighscores = std::vector<unsigned int>(3, 0);
  }
}

void LevelInfo::saveHighscores() const {
  std::ofstream stream("levels/" + mName + "/highsocres.txt", ios::trunc);

  for (unsigned int score : mHighscores)
    stream << score << std::endl;

  stream.close();
}

std::vector<unsigned int> LevelInfo::getHighscores() const { return mHighscores; }