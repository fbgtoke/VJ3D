#include "Scene.h"
#include "SceneMenu.h"
#include "SceneTest.h"
#include "SceneWin.h"
#include "SceneDead.h"

#include "Game.h"

const glm::vec3 Scene::kLightDirection = glm::normalize(glm::vec3(0.0, -4.0, -1.0));
const float Scene::kAmbientLight = 0.4f;

Scene::Scene() {}

Scene::~Scene() {}

Scene* Scene::create(SceneType type) {
  switch(type) {
  case SCENE_MENU: return new SceneMenu();
  case SCENE_WIN:  return new SceneWin();
  case SCENE_DEAD: return new SceneDead();
  default:
    return new SceneTest();
  }
}

void Scene::init() {
  initShaders();
  initScene();
  initGUI();
  mCurrentTime = 0.0f;
}

void Scene::update(int deltaTime) {
  Game::instance().scanKeys();
  mCurrentTime += deltaTime;

  updateScene(deltaTime);
  updateGUI(deltaTime);
}

void Scene::render() {
  renderScene();
  renderGUI();
}

void Scene::initShaders() {
  mTexProgram = Game::instance().getResource().shader("simple");
  mGuiProgram = Game::instance().getResource().shader("texture");
}

void Scene::initScene() {
  mProjectionMatrix = glm::mat4(1.0f);
  mViewMatrix = glm::mat4(1.0f);
}

void Scene::updateScene(int deltaTime) {}

void Scene::renderScene() {
  mTexProgram->use();
  mTexProgram->setUniformMatrix4f("PM", mProjectionMatrix);
  mTexProgram->setUniformMatrix4f("VM", mViewMatrix);
  mTexProgram->setUniform3f("lightDir", kLightDirection.x, kLightDirection.y, kLightDirection.z);
  mTexProgram->setUniform1f("ambientColor", kAmbientLight);
}

void Scene::initGUI() {
  mProjectionMatrixGUI = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
  mViewMatrixGUI = glm::mat4(1.0f);
}

void Scene::updateGUI(int deltaTime) {}

void Scene::renderGUI() {
  mGuiProgram->use();
  mGuiProgram->setUniformMatrix4f("PM", mProjectionMatrixGUI);
  mGuiProgram->setUniformMatrix4f("VM", mViewMatrixGUI);
  mGuiProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
  mGuiProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
}