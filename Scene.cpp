#include "Scene.h"
#include "SceneMenu.h"
#include "SceneTest.h"
#include "SceneWin.h"
#include "SceneDead.h"

#include "Game.h"

const glm::vec3 Scene::kLightDirection = glm::normalize(glm::vec3(0.0, -4.0, -1.0));
const float Scene::kAmbientLight = 0.4f;

Scene::Scene() {}

Scene::~Scene() {
  for (auto it = mSoundEffects.begin(); it != mSoundEffects.end(); ++it)
    delete (*it);
  mSoundEffects.clear();
}

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

  checkSoundEffects();
}

void Scene::render() {
  renderScene();
  renderGUI();
}

glm::vec3 Scene::getLightDirection() { return kLightDirection; }
float Scene::getAmbientLight() { return kAmbientLight; }

glm::mat4 Scene::getProjectionMatrix() const { return mProjectionMatrix; }
glm::mat4 Scene::getViewMatrix() const { return mViewMatrix; }

void Scene::initShaders() {
  mTexProgram = Game::instance().getResource().shader("simple");
  mGuiProgram = Game::instance().getResource().shader("texture");
}

void Scene::initScene() {
  mProjectionMatrix = glm::mat4(1.0f);
  mViewMatrix = glm::mat4(1.0f);
}

void Scene::updateScene(int deltaTime) {}

void Scene::renderScene() {}

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

void Scene::playSoundEffect(const std::string& name) {
  const sf::SoundBuffer* buffer = Game::instance().getResource().soundBuffer(name);

  if (buffer != nullptr) {
    sf::Sound* sound = new sf::Sound(*buffer);
    sound->play();
    mSoundEffects.push_back(sound);
  }
}

void Scene::checkSoundEffects() {
  auto it = mSoundEffects.begin();
  sf::Sound* sound = nullptr;

  while (it != mSoundEffects.end()) {
    sound = (*it);

    if (sound->getStatus() != sf::Sound::Playing) {
      delete sound;
      mSoundEffects.erase(it++);
    } else {
      it++;
    }
  }
}