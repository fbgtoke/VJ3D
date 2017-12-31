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

	float left = 6 * TILE_SIZE * (-1.f);
	float right = 6 * TILE_SIZE;
  float bottom = 2 * TILE_SIZE * (-1.f);
  float top = (float)SCREEN_HEIGHT/(float)SCREEN_WIDTH * (right - left) + bottom;
	float znear = 0.1f;
	float zfar  = 10000.f;
  mProjectionMatrix = glm::ortho(left, right, bottom, top, znear, zfar);

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

void SceneTest::beforeRender() {
  Scene::beforeRender();
  mFramebuffer.use();
}

void SceneTest::afterRender() {
  Scene::afterRender();
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  ShaderProgram* shader = Game::instance().getResource().shader("post");
  
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
  VRP.x = mLevel->getPlayer()->getCenter().x;
  VRP.y = 0.f;
  VRP.z += mCameraVel * (float)deltaTime;
  OBS = VRP + kObsVector * TILE_SIZE;

  mViewMatrix = glm::lookAt(OBS, VRP, UP);
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
  dir.x = cos(mLightAngle);
  dir.y = abs(sin(mLightAngle)) * (-1.f);
  dir.z = 0.f;
  dir = glm::normalize(dir);

  return dir;
}

float SceneTest::getAmbientLight() const {
  float maxAmbientLight = Game::instance().getResource().Float("maxAmbientLight");
  float minAmbientLight = Game::instance().getResource().Float("minAmbientLight");
  return abs(sin(mLightAngle)) * maxAmbientLight + minAmbientLight;
}