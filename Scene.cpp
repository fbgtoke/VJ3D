#include "Scene.h"
#include "SceneMenu.h"
#include "SceneLevelSelect.h"
#include "SceneTest.h"
#include "SceneWin.h"
#include "SceneDead.h"

#include "Game.h"

Scene::Scene(Scene::SceneType type)
  : mType(type) {}

Scene::~Scene() {
  for (auto it = mSoundEffects.begin(); it != mSoundEffects.end(); ++it)
    delete (*it);
  mSoundEffects.clear();

  for (auto it = mModels.begin(); it != mModels.end(); ++it) {
    if (!(*it)->hasBeenDestroyed())
      (*it)->destroy();

    delete (*it);
  }
  mModels.clear();
  mUpdateList.clear();
  mRenderList.clear();
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
Scene::SceneType Scene::getType() const { return mType; }

void Scene::init() {
  initShaders();
  initScene();
  mCurrentTime = 0.0f;
}

void Scene::update(int deltaTime) {
  Game::instance().scanKeys();
  mCurrentTime += deltaTime;

  updateScene(deltaTime);

  checkSoundEffects();
}

void Scene::render() {
  beforeRender();

  glm::mat4 PM = getProjectionMatrix();
  glm::mat4 VM = getViewMatrix();
  glm::mat4 TG;
  glm::mat4 MVP;

  glm::vec3 lightDirection = getLightDirection();
  glm::vec3 ambientColor = getAmbientColor();

  for (Model* model : mRenderList) {
    TG = model->getTransform();
    MVP = PM * VM * TG;

    mTexProgram->use();
    mTexProgram->setUniform3f("lightDir", lightDirection.x, lightDirection.y, lightDirection.z);
    mTexProgram->setUniform3f("ambientColor", ambientColor);
    mTexProgram->setUniform2f("texoffset", 0.f, 0.f);
    mTexProgram->setUniformMatrix4f("MVP", MVP);
    mTexProgram->setUniformMatrix4f("modelMatrix", TG);

    Mesh* mesh = model->getMesh();
    if (mesh != nullptr && model->visible()) {
      model->beforeRender();

      mesh->useShader(mTexProgram);
      glBindVertexArray(mesh->getVAO());
      glDrawArrays(GL_TRIANGLES, 0, mesh->numVertices());
      glBindVertexArray(0);

      model->afterRender();
    }
  }

  afterRender();
}

void Scene::receiveString(const std::string& tag, const std::string str) {}

ShaderProgram* Scene::getShader() { return mTexProgram; }

glm::vec3 Scene::getLightDirection() const { return kLightDirection; }
glm::vec3 Scene::getAmbientColor() const { return kAmbientColor; }

glm::mat4 Scene::getProjectionMatrix() const { return mProjectionMatrix; }
glm::mat4 Scene::getViewMatrix() const { return mViewMatrix; }

void Scene::initShaders() {
  mTexProgram = Game::instance().getResource().shader("simple");
}

void Scene::initScene() {
  mProjectionMatrix = glm::mat4(1.0f);
  mViewMatrix = glm::mat4(1.0f);

  kLightDirection = glm::vec3(0.f);

  kAmbientColor.x = Game::instance().getResource().Float("ambientColor_x");
  kAmbientColor.y = Game::instance().getResource().Float("ambientColor_y");
  kAmbientColor.z = Game::instance().getResource().Float("ambientColor_z");
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

void Scene::beforeRender() {
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Scene::afterRender() {}

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

  mModels.push_back(model);

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
    auto it = mModels.begin();
    while (it != mModels.end() && (*it) != model) ++it;
    if ((*it) == model) 
      mModels.erase(it);

    auto it2 = mUpdateList.begin();
    while (it2 != mUpdateList.end() && (*it2) != model) ++it2;
    if ((*it2) == model) 
      mUpdateList.erase(it2);

    auto it3 = mRenderList.begin();
    while (it3 != mRenderList.end() && (*it3) != model) ++it3;
    if ((*it3) == model)
      mRenderList.erase(it3);

    delete model;
  }
}