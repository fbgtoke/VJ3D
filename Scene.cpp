#include "Scene.h"
#include "SceneMenu.h"
#include "SceneLevelSelect.h"
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

  for (auto it = mModels.begin(); it != mModels.end(); ++it) {
    (*it)->destroy();
    delete (*it);
  }
  mModels.clear();
}

Scene* Scene::create(SceneType type) {
  switch(type) {
  case SCENE_MENU: return new SceneMenu();
  case SCENE_LEVEL_SELECT: return new SceneLevelSelect();
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

void Scene::updateScene(int deltaTime) {
  auto it = mUpdateList.begin();
  while (it != mUpdateList.end()) {
    Model* model = (*it);

    if (model->hasBeenDestroyed()) {
      it++;
      removeModel(model);
    } else {
      model->update(deltaTime);

      if (model->checkCollisions()) {
        for (Model* model2 : mModels) {
          if (model != model2 && model->collides(model2))
            model->onCollision(model2);
        }
      }
      it++;
    }
  }
}

void Scene::renderScene() {
  for (Model* model : mRenderList)
    model->render();
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

void Scene::addModel(Model* model, unsigned char flags) {
  if (model == nullptr) return;

  mModels.insert(model);

  if (flags & Scene::UpdateFirst)
    mUpdateList.push_front(model);
  else if (flags & Scene::UpdateLast)
    mUpdateList.push_back(model);

  if (flags & Scene::RenderFirst)
    mRenderList.push_front(model);
  else if (flags & Scene::RenderLast)
    mRenderList.push_back(model);
}

void Scene::removeModel(Model* model) {
  if (model != nullptr) {
    mModels.erase(model);

    auto it = mUpdateList.begin();
    while (it != mUpdateList.end() && (*it) != model) ++it;
    if ((*it) == model) 
      mUpdateList.erase(it);

    auto it2 = mRenderList.begin();
    while (it2 != mRenderList.end() && (*it2) != model) ++it2;
    if ((*it2) == model)
      mRenderList.erase(it2);

    delete model;
  }
}