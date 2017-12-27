#include "SceneLevelSelect.h"
#include "Game.h"

SceneLevelSelect::SceneLevelSelect() {}

SceneLevelSelect::~SceneLevelSelect() {}

void SceneLevelSelect::initScene() {
  Scene::initScene();

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

  mFrame = new Model();
  mFrame->init();
  mFrame->setMesh(Game::instance().getResource().mesh("plane.obj"));
  mFrame->setTexture(Game::instance().getResource().texture("frame.png"));
  mFrame->setPosition(mLevels[0]->getPosition());
  mFrame->setRotation(glm::vec3((float)M_PI * 0.5f, 0.f, 0.f));
  mFrame->setScale(glm::vec3(1.25f));
  addModel(mFrame);

  mCurrentSelected = 0;
}

void SceneLevelSelect::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);
  else if (Game::instance().getKeyPressed('z'))
    Game::instance().changeScene(Scene::SCENE_TEST);

  if (Game::instance().getKeyPressed('w'))
    Game::instance().changeScene(Scene::SCENE_TEST);
  else if (Game::instance().getKeyPressed('a'))
    prevIndex();
  else if (Game::instance().getKeyPressed('d'))
    nextIndex();

  mFrame->setPosition(index2position(mCurrentSelected));
}

void SceneLevelSelect::renderScene() {
  Scene::renderScene();
}

void SceneLevelSelect::initLevelList() {
  DIR* dir;
  struct dirent *ent;
  if ((dir = opendir("levels")) != nullptr) {
    while ((ent = readdir(dir)) != nullptr) {
      std::string entry = ent->d_name;
      if (entry != "." && entry != "..")
        addLevel(entry);
    }
    closedir(dir);
  }
}

void SceneLevelSelect::addLevel(const std::string& name) {
  Model* model = new Model();
  model->init();
  model->setMesh(Game::instance().getResource().mesh("plane.obj"));
  model->setTexture(Game::instance().getResource().texture("error.png"));
  model->setRotation(glm::vec3((float)M_PI * 0.5f, 0.f, 0.f));

  glm::vec3 position = index2position(mLevels.size());
  model->setPosition(position);

  addModel(model);
  mLevels.push_back(model);
}

void SceneLevelSelect::nextIndex() {
  if (mCurrentSelected >= mLevels.size() - 1)
    mCurrentSelected = 0;
  else
    mCurrentSelected += 1;
}

void SceneLevelSelect::prevIndex() {
  if (mCurrentSelected == 0)
    mCurrentSelected = mLevels.size() - 1;
  else
    mCurrentSelected -= 1;
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