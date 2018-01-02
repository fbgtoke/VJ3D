#include "SceneTest.h"
#include "Game.h"

SceneTest::SceneTest()
  : Scene(Scene::SCENE_TEST), mLevelName("") {}

SceneTest::~SceneTest() {
  if (mLevel != nullptr)
    delete mLevel;
}

void SceneTest::receiveString(const std::string& tag, const std::string str) {
  if (tag == "level-name")
    mLevelName = str;
}

void SceneTest::removeModel(Model* model) {
  Obstacle* obstacle = dynamic_cast<Obstacle*>(model);
  if (obstacle != nullptr)
    mLevel->removeObstacle(obstacle);

  Scene::removeModel(model);
}

void SceneTest::initScene() {
	Scene::initScene();

  if (mLevelName == "") {
    std::cout << "No level selected!" << std::endl;
    Game::instance().changeScene(Scene::SCENE_MENU);
    return;
  }

  kObsVector.x = Game::instance().getResource().Float("ObsVector_x");
  kObsVector.y = Game::instance().getResource().Float("ObsVector_y");
  kObsVector.z = Game::instance().getResource().Float("ObsVector_z");
  kCameraVel = Game::instance().getResource().Float("CameraVel");

  mLevel = LevelGenerator::generate("levels/" + mLevelName + "/");

  mCameraVel = kCameraVel;
  VRP = mLevel->getPlayer()->getCenter();
  OBS = VRP + kObsVector * TILE_SIZE;

  mLightAngle = 0.f;

  mFramebuffer.init();
}

void SceneTest::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);

  updateCamera(deltaTime);
  mLevel->update(deltaTime);

  checkPlayerOutOfCamera();
  checkPlayerDead();

  mLightAngle += (float)deltaTime * Game::instance().getResource().Float("sunSpeed");
}

void SceneTest::render() {
  renderShadowmap();
  renderFramebuffer();
  renderScene();
}

void SceneTest::renderShadowmap() {
  mTexProgram = Game::instance().getResource().shader("depth");
  mDepthbuffer.use();

  glm::mat4 PM = getDepthProjectionMatrix();
  glm::mat4 VM = getDepthViewMatrix();
  glm::mat4 TG;
  glm::mat4 MVP;
  
  for (Model* model : mRenderList) {
    TG = model->getTransform();
    MVP = PM * VM * TG;

    mTexProgram->use();
    mTexProgram->setUniformMatrix4f("MVP", MVP);

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
}

void SceneTest::renderFramebuffer() {
  mTexProgram = Game::instance().getResource().shader("level");
  mFramebuffer.use();

  glm::mat4 PM = getProjectionMatrix();
  glm::mat4 VM = getViewMatrix();
  glm::mat4 TG;
  glm::mat4 MVP;

  glm::mat4 biasMatrix = glm::mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 0.1
  );
  glm::mat4 depthPM = getDepthProjectionMatrix();
  glm::mat4 depthVM = getDepthViewMatrix();
  glm::mat4 depthMVP;

  glm::vec3 lightDirection = getLightDirection();
  glm::vec3 ambientColor = getAmbientColor();
  
  for (Model* model : mRenderList) {
    TG = model->getTransform();

    MVP = PM * VM * TG;
    mTexProgram->setUniformMatrix4f("MVP", MVP);
    depthMVP = biasMatrix * depthPM * depthVM * TG;
    mTexProgram->setUniformMatrix4f("depthMVP", depthMVP);

    mTexProgram->use();
    mTexProgram->setUniform3f("lightDir", lightDirection.x, lightDirection.y, lightDirection.z);
    mTexProgram->setUniform3f("ambientColor", ambientColor);
    mTexProgram->setUniform2f("texoffset", 0.f, 0.f);

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
}

void SceneTest::renderScene() {
  ShaderProgram* shader = Game::instance().getResource().shader("post");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 TG(1.f);
  TG = glm::scale(TG, glm::vec3(-1.f, 1.f, 1.f));

  shader->use();
  shader->setUniformMatrix4f("TG", TG);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, mFramebuffer.getTexture());

  Mesh* quad = Game::instance().getResource().mesh("frame-buffer.obj");
  quad->useShader(shader);

  glBindVertexArray(quad->getVAO());
  glDrawArrays(GL_TRIANGLES, 0, quad->numVertices());
  glBindVertexArray(0);
  glDisable(GL_TEXTURE_2D);
}

void SceneTest::updateCamera(int deltaTime) {
  if (mLevel == nullptr || mLevel->getPlayer() == nullptr)
    return;
  
  if (mLevel->getPlayer()->isAlive()) {
    VRP.x = mLevel->getPlayer()->getCenter().x;
    VRP.y = 0.f;
    VRP.z += mCameraVel * (float)deltaTime;
    OBS = VRP + kObsVector * TILE_SIZE;
  }
}

void SceneTest::checkPlayerDead() {
  Player* player = mLevel->getPlayer();
  if (player == nullptr) return;

  if (player->isExploding())
    mCameraVel = 0.f;
  if (player->isDead()) {
    Game::instance().changeScene(Scene::SCENE_DEAD);
    Game::instance().getBufferedScene()->receiveString("level-name", mLevelName);
  }
}

void SceneTest::checkPlayerOutOfCamera() {
  glm::vec4 homoPosition(mLevel->getPlayer()->getCenter(), 1.0f);
  glm::vec4 projectedPosition =
    glm::vec4(mProjectionMatrix * mViewMatrix * homoPosition);

  if (projectedPosition.y/projectedPosition.w < -1.0f)
    mLevel->getPlayer()->explode();
}

glm::vec3 SceneTest::getLightDirection() const {
  glm::vec3 dir;
  dir.z = cos(mLightAngle);
  dir.y = abs(sin(mLightAngle)) * (-1.f);
  dir.x = 0.f;

  dir = glm::normalize(dir);

  return dir;
}

glm::mat4 SceneTest::getProjectionMatrix() const {
  return glm::ortho(
    6 * TILE_SIZE * (-1.f),
    6 * TILE_SIZE,
    2 * TILE_SIZE * (-1.f),
    (float)SCREEN_HEIGHT/(float)SCREEN_WIDTH * (12 * TILE_SIZE) - 2 * TILE_SIZE,
    0.1f,
    10000.f
  );
}

glm::mat4 SceneTest::getDepthProjectionMatrix() const {
  return glm::ortho(-10.f, 10.f, -10.f, 10.f, -10.f, 20.f);
}

glm::mat4 SceneTest::getViewMatrix() const {
  return glm::lookAt(OBS, VRP, UP);
}

glm::mat4 SceneTest::getDepthViewMatrix() const {
  glm::vec3 obs = 2 * TILE_SIZE * getLightDirection() * (-1.f);
  glm::vec3 vrp = glm::vec3(0.f);

  return glm::lookAt(obs, vrp, UP);
}