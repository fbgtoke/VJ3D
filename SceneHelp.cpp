#include "SceneHelp.h"
#include "Game.h"

SceneHelp::SceneHelp()
  : Scene(Scene::SCENE_HELP) {}

SceneHelp::~SceneHelp() {
  for (int i = 0; i < 3; ++i) {
    for (Model* model : mModels[i])
      delete model;
    mModels[i].clear();
  }
}

void SceneHelp::init() {
  Scene::init();
  mTexProgram = Game::instance().getResource().shader("simple");

  
  initLayer0();
  initLayer1();
  initLayer2();

  mCamera.init();
  mCamera.follow(mModels[0][2]);
  mCamera.move(OUT * 3.f * TILE_SIZE + LEFT * 1.f * TILE_SIZE);

  mSun.init();
  mSun.setCurrentTime((float)M_PI * 0.75f);

  mCurrentLayer = 0;
}

void SceneHelp::initLayer0() {
  Model* tile;
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      tile = new Model();
      tile->init();
      tile->setMesh(Game::instance().getResource().mesh("cube.obj"));
      tile->setTexture(Game::instance().getResource().texture("grass.png"));
      tile->setPositionInTiles(glm::vec3((float)j - 1.f, -1.f, (float)i - 1.f));
      mModels[0].push_back(tile);
    }
  }

  Player* cowboy = new Player();
  cowboy->init();
  cowboy->setPositionInTiles(glm::vec3(0.f));
  mModels[0].push_back(cowboy);

  mCountdown = 1000;
}

void SceneHelp::initLayer1() {
  Model* cactus = new Model();
  cactus->init();
  cactus->setMesh(Game::instance().getResource().mesh("cactus.obj"));
  cactus->setTexture(Game::instance().getResource().texture("palette.png"));
  cactus->setPositionInTiles(glm::vec3(-4.f, 0.f, 2.f));
  mModels[1].push_back(cactus);

  Model* water = new Model();
  water->init();
  water->setMesh(Game::instance().getResource().mesh("cube.obj"));
  water->setTexture(Game::instance().getResource().texture("water.png"));
  water->setPositionInTiles(glm::vec3(-2.f, -1.f, 1.f));
  mModels[1].push_back(water);

  Model* cowboy = new Model();
  cowboy->init();
  cowboy->setMesh(Game::instance().getResource().mesh("drowningcowboy1.obj"));
  cowboy->setTexture(Game::instance().getResource().texture("palette.png"));
  cowboy->setPositionInTiles(glm::vec3(-2.f, 0.f, 1.f));
  mModels[1].push_back(cowboy);

  Model* horse = new Model();
  horse->init();
  horse->setMesh(Game::instance().getResource().mesh("animationhorse1.obj"));
  horse->setTexture(Game::instance().getResource().texture("palette.png"));
  horse->setPositionInTiles(glm::vec3(0.f, 0.f, 1.f));
  horse->setRotation(glm::vec3(0.f, (float)M_PI * -0.5f, 0.f));
  mModels[1].push_back(horse);

  Model* carriage = new Model();
  carriage->init();
  carriage->setMesh(Game::instance().getResource().mesh("carriage1.obj"));
  carriage->setTexture(Game::instance().getResource().texture("palette.png"));
  carriage->setPositionInTiles(glm::vec3(2.f, 0.f, 1.f));
  carriage->setRotation(glm::vec3(0.f, (float)M_PI, 0.f));
  mModels[1].push_back(carriage);
}

void SceneHelp::initLayer2() {
  Model* stone = new Model();
  stone->init();
  stone->setMesh(Game::instance().getResource().mesh("stone.obj"));
  stone->setTexture(Game::instance().getResource().texture("palette.png"));
  stone->setPositionInTiles(glm::vec3(-2.f, 0.f, -2.f));
  mModels[2].push_back(stone);

  Model* boat = new Model();
  boat->init();
  boat->setMesh(Game::instance().getResource().mesh("boat.obj"));
  boat->setTexture(Game::instance().getResource().texture("palette.png"));
  boat->setPositionInTiles(glm::vec3(0.f, 0.f, -2.f));
  mModels[2].push_back(boat);

  Model* bonus = new Model();
  bonus->init();
  bonus->setMesh(Game::instance().getResource().mesh("bonus1.obj"));
  bonus->setTexture(Game::instance().getResource().texture("palette.png"));
  bonus->setPositionInTiles(glm::vec3(0.f, 0.f, 1.f));
  bonus->setRotationSpeed(glm::vec3(0.f, 0.002f, 0.f));
  mModels[2].push_back(bonus);

  Model* goal;
  goal = new Model();
  goal->init();
  goal->setMesh(Game::instance().getResource().mesh("cube.obj"));
  goal->setTexture(Game::instance().getResource().texture("goal.png"));
  goal->setPositionInTiles(glm::vec3(0.f, -1.f, 3.f));
  mModels[2].push_back(goal);

  goal = new Model();
  goal->init();
  goal->setMesh(Game::instance().getResource().mesh("cube.obj"));
  goal->setTexture(Game::instance().getResource().texture("goal.png"));
  goal->setPositionInTiles(glm::vec3(1.f, -1.f, 3.f));
  mModels[2].push_back(goal);

  goal = new Model();
  goal->init();
  goal->setMesh(Game::instance().getResource().mesh("cube.obj"));
  goal->setTexture(Game::instance().getResource().texture("goal.png"));
  goal->setPositionInTiles(glm::vec3(2.f, -1.f, 3.f));
  mModels[2].push_back(goal);
}

void SceneHelp::updateLayer0(int deltaTime) {
  Player* cowboy = dynamic_cast<Player*>(mModels[0][mModels[0].size() - 1]);

  if (cowboy->getState() != Player::Idle) return;

  mCountdown -= deltaTime;
  if (mCountdown > 0) return;
  mCountdown = 1000;

  if (cowboy->getPositionInTiles() == glm::vec3(0.f)) {
    cowboy->moveTowards(cowboy->getPosition() + LEFT * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_white.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_red.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_white.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_white.png"));
  }
  else if (cowboy->getPositionInTiles() == LEFT) {
    cowboy->moveTowards(cowboy->getPosition() + IN * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_red.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_white.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_white.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_white.png"));
  }
  else if (cowboy->getPositionInTiles() == LEFT + IN) {
    cowboy->moveTowards(cowboy->getPosition() + RIGHT * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_white.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_white.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_white.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_red.png"));
  }
  else if (cowboy->getPositionInTiles() == IN) {
    cowboy->moveTowards(cowboy->getPosition() + RIGHT * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_white.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_white.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_white.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_red.png"));
  }
  else if (cowboy->getPositionInTiles() == RIGHT + IN) {
    cowboy->moveTowards(cowboy->getPosition() + OUT * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_white.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_white.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_red.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_white.png"));
  }
  else if (cowboy->getPositionInTiles() == RIGHT) {
    cowboy->moveTowards(cowboy->getPosition() + OUT * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_white.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_white.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_red.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_white.png"));
  }
  else if (cowboy->getPositionInTiles() == RIGHT + OUT) {
    cowboy->moveTowards(cowboy->getPosition() + LEFT * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_white.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_red.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_white.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_white.png"));
  }
  else if (cowboy->getPositionInTiles() == OUT) {
    cowboy->moveTowards(cowboy->getPosition() + LEFT * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_white.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_red.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_white.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_white.png"));
  }
  else if (cowboy->getPositionInTiles() == LEFT + OUT) {
    cowboy->moveTowards(cowboy->getPosition() + IN * TILE_SIZE);
    mGui->getSprite("W")->setTexture(Game::instance().getResource().texture("W_red.png"));
    mGui->getSprite("A")->setTexture(Game::instance().getResource().texture("A_white.png"));
    mGui->getSprite("S")->setTexture(Game::instance().getResource().texture("S_white.png"));
    mGui->getSprite("D")->setTexture(Game::instance().getResource().texture("D_white.png"));
  }
}

void SceneHelp::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close)) // Escape
    Game::instance().changeScene(Scene::SCENE_MENU);

  if (InputManager::getAction(InputManager::Accept)) {
    if (mCurrentLayer < 2)
      nextLayer();
    else
      Game::instance().changeScene(Scene::SCENE_MENU);
  }

  for (Model* model : mModels[mCurrentLayer])
    model->update(deltaTime);

  if (mCurrentLayer == 0) updateLayer0(deltaTime);
}

void SceneHelp::render() {
  Scene::render();

  for (Model* model : mModels[mCurrentLayer])
    model->render();
}

glm::vec3 SceneHelp::getLightDirection() const { return glm::vec3(-.5f, -1.f, -.5f); }
glm::mat4 SceneHelp::getProjectionMatrix() const { return mCamera.getProjectionMatrix(); }
glm::mat4 SceneHelp::getViewMatrix() const { return mCamera.getViewMatrix(); }

void SceneHelp::initGui() {
  Scene::initGui();

  mGui = Game::instance().getResource().layout("help.xml");
  mGui->showLayer(0);
  mGui->hideLayer(1);
  mGui->hideLayer(2);
}

void SceneHelp::nextLayer() {
  mGui->hideLayer(mCurrentLayer);

  mCurrentLayer = (mCurrentLayer + 1)%3;
  mGui->showLayer(mCurrentLayer);
}