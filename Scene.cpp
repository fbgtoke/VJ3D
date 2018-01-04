#include "Scene.h"
#include "SceneMenu.h"
#include "SceneLevelSelect.h"
#include "SceneTest.h"
#include "SceneWin.h"
#include "SceneDead.h"
#include "SceneHelp.h"
#include "SceneCredits.h"

#include "Game.h"

Scene::Scene(Scene::SceneType type)
  : mType(type), mGui(nullptr) {}

Scene::~Scene() {
  for (auto it = mSoundEffects.begin(); it != mSoundEffects.end(); ++it)
    delete (*it);
  mSoundEffects.clear();

  for (auto it = mParticles.begin(); it != mParticles.end(); ++it)
    delete (*it);
  mParticles.clear();

  if (mGui != nullptr)
    delete mGui;
}

Scene* Scene::create(SceneType type) {
  switch(type) {
  case SCENE_MENU: return new SceneMenu();
  case SCENE_LEVEL_SELECT: return new SceneLevelSelect();
  case SCENE_WIN:  return new SceneWin();
  case SCENE_DEAD: return new SceneDead();
  case SCENE_HELP: return new SceneHelp();
  case SCENE_CREDITS: return new SceneCredits();
  default:
    return new SceneTest();
  }
}
Scene::SceneType Scene::getType() const { return mType; }

void Scene::init() {
  mCurrentTime = 0.0f;

  mTexProgram = Game::instance().getResource().shader("simple");

  mProjectionMatrix = glm::mat4(1.0f);
  mViewMatrix = glm::mat4(1.0f);

  kLightDirection = glm::vec3(0.f);

  kAmbientColor.x = Game::instance().getResource().Float("ambientColor_x");
  kAmbientColor.y = Game::instance().getResource().Float("ambientColor_y");
  kAmbientColor.z = Game::instance().getResource().Float("ambientColor_z");

  initGui();
}

void Scene::update(int deltaTime) {
  std::cout << deltaTime << std::endl;;
  Game::instance().scanKeys();
  mCurrentTime += deltaTime;

  checkSoundEffects();
  checkParticles(deltaTime);

  updateGui();
}

void Scene::render() {
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  mTexProgram->use();

  glm::mat4 PM = getProjectionMatrix();
  mTexProgram->setUniformMatrix4f("PM", PM);
  glm::mat4 VM = getViewMatrix();
  mTexProgram->setUniformMatrix4f("VM", VM);

  glm::vec3 lightDirection = getLightDirection();
  mTexProgram->setUniform3f("lightDir", lightDirection);
  glm::vec3 ambientColor = getAmbientColor();
  mTexProgram->setUniform3f("ambientColor", ambientColor);

  for (Particle* particle : mParticles)
    particle->render();

  renderGui();
}

void Scene::receiveString(const std::string& tag, const std::string str) {}

ShaderProgram* Scene::getShader() { return mTexProgram; }

glm::vec3 Scene::getLightDirection() const { return kLightDirection; }
glm::vec3 Scene::getAmbientColor() const { return kAmbientColor; }

glm::mat4 Scene::getProjectionMatrix() const { return mProjectionMatrix; }
glm::mat4 Scene::getViewMatrix() const { return mViewMatrix; }

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

void Scene::addParticle(Particle* particle) {
  if (particle != nullptr) mParticles.push_back(particle);
}

void Scene::checkParticles(int deltaTime) {
  auto it = mParticles.begin();
  Particle* particle = nullptr;

  while (it != mParticles.end()) {
    particle = (*it);

    if (particle->hasBeenDestroyed()) {
      delete particle;
      mParticles.erase(it++);
    } else {
      particle->update(deltaTime);
      it++;
    }
  }
}

bool Scene::outOfCamera(Model* model) const {
  glm::vec4 homoPosition(model->getCenter(), 1.0f);
  glm::vec4 projectedPosition =
    glm::vec4(getProjectionMatrix() * getViewMatrix() * homoPosition);

  return projectedPosition.y/projectedPosition.w < -1.0f;
}

bool Scene::doUpdate(Model* model) const { return true; }

void Scene::initGui() {}

void Scene::updateGui() {}

void Scene::renderGui() {
  if (mGui != nullptr)
    mGui->render();
}