#include "SceneLevelSelect.h"
#include "Game.h"

const int SceneLevelSelect::kLevelsPerRow = 5;

SceneLevelSelect::SceneLevelSelect() {}

SceneLevelSelect::~SceneLevelSelect() {}

void SceneLevelSelect::initScene() {
  Scene::initScene();

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
}

void SceneLevelSelect::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);
  else if (Game::instance().getKeyPressed('z'))
    Game::instance().changeScene(Scene::SCENE_TEST);

  if (Game::instance().getKeyPressed('w'))
    Game::instance().changeScene(Scene::SCENE_TEST);
  else if (Game::instance().getKeyPressed('s'))
    Game::instance().changeScene(Scene::SCENE_TEST);
  else if (Game::instance().getKeyPressed('a'))
    Game::instance().changeScene(Scene::SCENE_TEST);
  else if (Game::instance().getKeyPressed('d'))
    Game::instance().changeScene(Scene::SCENE_TEST);
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

  const float column = (float)(mLevels.size()%kLevelsPerRow);
  const float row = (float)(mLevels.size()/kLevelsPerRow);

  const float column_margin = (1.5f * model->getSize().x);
  const float row_margin = (1.5f * model->getSize().x) * (-1.f);

  const float column_offet = column_margin * 1.5f;
  const float row_offset = row_margin * 2.5f * (-1.f);

  glm::vec3 position;
  position.x = column * column_margin + column_offet;
  position.y = row * row_margin + row_offset;
  position.z = 0.f;
  model->setPosition(position);

  addModel(model);
  mLevels.push_back(model);
}