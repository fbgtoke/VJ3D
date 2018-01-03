#include "SceneLevelSelect.h"
#include "Game.h"

SceneLevelSelect::SceneLevelSelect()
  : Scene(Scene::SCENE_LEVEL_SELECT), mFrame(nullptr) {}

SceneLevelSelect::~SceneLevelSelect() {
  delete mFrame;
  for (Model* model : mThumbnails) delete model;
  delete mHighscoreTitle;
  for (Text3D* text : mHighscoreTable) delete text;
}

void SceneLevelSelect::init() {
  Scene::init();

  mTexProgram = Game::instance().getResource().shader("quad");

  kLevelsPerRow = Game::instance().getResource().Int("LevelsPerRow");
  kOptionSize = Game::instance().getResource().Float("OptionSize");

  float left = 0.f;
  float right = SCREEN_WIDTH * 0.125f;
  float bottom = 0.f;
  float top = SCREEN_HEIGHT * 0.125f;
  float znear = 0.1f;
  float zfar  = 10000.f;
  mProjectionMatrix = glm::ortho(left, right, bottom, top, znear, zfar);

  VRP = glm::vec3(0.f, 0.f, -1);
  OBS = glm::vec3(0.f, 0.f, 5);
  mViewMatrix = glm::lookAt(OBS, VRP, UP);

  initLevelList();
  mCurrentSelected = 0;

  initHighscoreText();

  mFrame = new Model();
  mFrame->init();
  mFrame->setMesh(Game::instance().getResource().mesh("plane.obj"));
  mFrame->setTexture(Game::instance().getResource().texture("frame.png"));
  mFrame->setPosition(index2position(mCurrentSelected));
  mFrame->setRotation(glm::vec3((float)M_PI * 0.5f, 0.f, 0.f));
  mFrame->setScale(glm::vec3(1.25f));
}

void SceneLevelSelect::update(int deltaTime) {
  Scene::update(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);
  
  if (Game::instance().getKeyPressed('z')) {
    Game::instance().changeScene(Scene::SCENE_TEST);

    std::string levelName = mLevels[mCurrentSelected].getName();
    Game::instance().getBufferedScene()->receiveString("level-name", levelName);
  }

  if (Game::instance().getKeyPressed('a'))
    prevIndex();
  else if (Game::instance().getKeyPressed('d'))
    nextIndex();

  mFrame->setPosition(index2position(mCurrentSelected));
}

void SceneLevelSelect::render() {
  Scene::render();

  /*for (Model* model : mThumbnails)
    model->render();

  mFrame->render();

  mHighscoreTitle->render();
  for (Text3D* text : mHighscoreTable)
    text->render();*/
}

void SceneLevelSelect::initLevelList() {
  std::ifstream stream("levels.txt");
  
  if (!stream.is_open()) {
    std::cout << "Could not find level definition file" << std::endl;
    Game::instance().stop();
    return;
  }

  std::string line;
  while (getline(stream, line))
    addLevel(line);
}

void SceneLevelSelect::addLevel(const std::string& name) {
  LevelInfo levelInfo;
  levelInfo.setName(name);
  mLevels.push_back(levelInfo);

  Model* model = new Model();
  model->init();
  model->setMesh(Game::instance().getResource().mesh("plane.obj"));
  model->setTexture(levelInfo.getThumbnail());
  model->setRotation(glm::vec3((float)M_PI * 0.5f, 0.f, 0.f));

  glm::vec3 position = index2position(mLevels.size() - 1);
  model->setPosition(position);
  mThumbnails.push_back(model);
}

void SceneLevelSelect::initHighscoreText() {
  mHighscoreTitle = new Text3D();
  mHighscoreTitle->setString("Highscores");
  mHighscoreTitle->setPosition(index2position(0) + DOWN * kOptionSize);

  std::vector<unsigned int> highscores = mLevels[mCurrentSelected].getHighscores();
  for (int i = 0; i < highscores.size(); ++i) {
    glm::vec3 position = index2position(0) + DOWN * kOptionSize * 2.f;
    position.y -= kOptionSize * 0.5f * (float)i;

    Text3D* highscore = new Text3D();
    highscore->setPosition(position);
    mHighscoreTable.push_back(highscore);
  }

  updateHighscoreTable();
}

void SceneLevelSelect::updateHighscoreTable() {
  std::vector<unsigned int> highscores = mLevels[mCurrentSelected].getHighscores();
  for (int i = 0; i < highscores.size(); ++i) {
    unsigned int score = highscores[i];

    std::string str;
    if (i == 0) str = "1st ";
    else if (i == 1) str = "2nd ";
    else str = "3rd ";
    str += std::to_string(score);

    mHighscoreTable[i]->setString(str);
  }
}

void SceneLevelSelect::nextIndex() {
  if (mCurrentSelected >= mLevels.size() - 1)
    mCurrentSelected = 0;
  else
    mCurrentSelected += 1;

  updateHighscoreTable();
}

void SceneLevelSelect::prevIndex() {
  if (mCurrentSelected == 0)
    mCurrentSelected = mLevels.size() - 1;
  else
    mCurrentSelected -= 1;

  updateHighscoreTable();
}

glm::vec3 SceneLevelSelect::index2position(unsigned int index) const {
  const float column = (float)(index%kLevelsPerRow);
  const float row = (float)(index/kLevelsPerRow);

  const float column_margin = (1.5f * kOptionSize);
  const float row_margin = (1.5f * kOptionSize) * (-1.f);

  const float column_offet = column_margin * 1.5f;
  const float row_offset = row_margin * 2.5f * (-1.f);

  glm::vec3 position;
  position.x = column * column_margin + column_offet;
  position.y = row * row_margin + row_offset;
  position.z = 0.f;

  return position;
}

void SceneLevelSelect::initGui() {
  Scene::initGui();

  mGui = Game::instance().getResource().layout("level-select.xml");
}

void SceneLevelSelect::updateGui() {
  Scene::updateGui();

}