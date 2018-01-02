#include "SceneTest.h"
#include "Game.h"

SceneTest::SceneTest()
  : Scene(Scene::SCENE_TEST), mLevel(nullptr), mLevelName(""), mPlayer(nullptr) {}

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

  mLevel = LevelGenerator::generate("levels/" + mLevelName + "/");
  initPlayer();

  initCamera();
  mLightAngle = 0.f;

  mFramebuffer.init();
  mDepthbuffer.init();
}

void SceneTest::initCamera() {
  kObsVector.x = Game::instance().getResource().Float("ObsVector_x");
  kObsVector.y = Game::instance().getResource().Float("ObsVector_y");
  kObsVector.z = Game::instance().getResource().Float("ObsVector_z");
  kCameraVel = Game::instance().getResource().Float("CameraVel");

  mCameraVel = kCameraVel;
  VRP = mPlayer->getCenter();
  OBS = VRP + kObsVector * TILE_SIZE;
}

void SceneTest::initPlayer() {
  mPlayer = new Player();
  mPlayer->init();

  addModel(mPlayer);

  if (mLevel != nullptr) {
    unsigned int width = mLevel->getTilemap().getWidth();
    float middleTile = (float)(width/2);
    mPlayer->setPositionInTiles(glm::vec3(middleTile, 0.f, 0.f));
  }
}

void SceneTest::updateScene(int deltaTime) {
  Scene::updateScene(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);

  updateCamera(deltaTime);
  
  checkPlayerInput();
  checkPlayerOutOfCamera();
  checkPlayerStandingTile();
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
  if (mPlayer != nullptr && mPlayer->isAlive()) {
    VRP.x = mPlayer->getCenter().x;
    VRP.y = 0.f;
    VRP.z += mCameraVel * (float)deltaTime;
    OBS = VRP + kObsVector * TILE_SIZE;
  }
}

void SceneTest::checkPlayerInput() {
  Player::State state = mPlayer->getState();
  if (state != Player::Idle && state != Player::onBoat) return;
  
  glm::vec3 direction(0.f);
  if (Game::instance().getKeyPressed('a') && state != Player::onBoat)
    direction = LEFT;
  else if (Game::instance().getKeyPressed('d') && state != Player::onBoat)
    direction = RIGHT;
  else if (Game::instance().getKeyPressed('w'))
    direction = IN;
  else if (Game::instance().getKeyPressed('s'))
    direction = OUT;

  /* Check for boat */
  glm::vec3 adjacent[3][3];
  mPlayer->getAdjacentTiles(adjacent);
  Obstacle* boat = nullptr;
  if (direction == IN) {
    boat = mLevel->getObstacleAtTile(adjacent[0][0]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel->getObstacleAtTile(adjacent[0][1]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel->getObstacleAtTile(adjacent[0][2]);
  } else if (direction == OUT) {
    boat = mLevel->getObstacleAtTile(adjacent[2][0]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel->getObstacleAtTile(adjacent[2][1]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel->getObstacleAtTile(adjacent[2][2]);
  }

  if (boat != nullptr) {
    glm::vec2 playerPos(mPlayer->getPosition().x, mPlayer->getPosition().z);
    glm::vec2 boatPos(boat->getPosition().x, boat->getPosition().z);
    float maxDistance = Game::instance().getResource().Float("maxBoatDistance");
    float distance = glm::distance(playerPos, boatPos);

    if (distance > maxDistance) boat = nullptr;
  }

  /* Check for stone */
  Obstacle* stone = nullptr;
  if (direction == IN)
    stone = mLevel->getObstacleAtTile(adjacent[0][1]);
  else if (direction == OUT)
    stone = mLevel->getObstacleAtTile(adjacent[0][2]);

  glm::vec3 targetTile = mPlayer->getPositionInTiles() + direction;
  glm::ivec2 targetTileInTilemap = mLevel->player2tilemap(targetTile);
  bool jumping = (direction != glm::vec3(0.f));
  bool outOfBounds = mLevel->getTilemap().outOfBounds(targetTileInTilemap);

  glm::vec3 targetPosition;
  if (jumping && !outOfBounds) {
    if (boat != nullptr && boat->getType() == Obstacle::Boat)  {
      mPlayer->moveTowardsBoat(boat);
    } else if (stone != nullptr && stone->getType() == Obstacle::Stone) {
      targetPosition = stone->getTopCenter();
      targetPosition.y += mPlayer->getSize().y * 0.5f;
      mPlayer->moveTowards(targetPosition);
    } else {
      targetPosition = mPlayer->getPosition() + direction * TILE_SIZE;
      targetPosition.y = mPlayer->getSize().y * 0.5f;
      mPlayer->moveTowards(targetPosition);
    }
  }
}

void SceneTest::checkPlayerOutOfCamera() {
  glm::vec4 homoPosition(mPlayer->getCenter(), 1.0f);
  glm::vec4 projectedPosition =
    glm::vec4(mProjectionMatrix * mViewMatrix * homoPosition);

  if (projectedPosition.y/projectedPosition.w < -1.0f)
    mPlayer->explode();
}

void SceneTest::checkPlayerStandingTile() {
  if (mPlayer != nullptr && mPlayer->getState() == Player::Idle) {
    glm::vec3 standingTile = mPlayer->getPositionInTiles();
    glm::ivec2 standingTileInTilemap = mLevel->player2tilemap(standingTile);
    Tile::Type tile = mLevel->getTilemap().getTile(standingTileInTilemap);

    if (tile == Tile::Water && !mLevel->obstacleOfTypeAtTile(Obstacle::Stone, standingTile))
      mPlayer->changeState(Player::Drowning);
    else if (tile == Tile::Goal)
      Game::instance().changeScene(Scene::SCENE_WIN);
  }
}

void SceneTest::checkPlayerDead() {
  if (mPlayer == nullptr) return;

  if (mPlayer->isExploding())
    mCameraVel = 0.f;
  if (mPlayer->isDead()) {
    Game::instance().changeScene(Scene::SCENE_DEAD);
    Game::instance().getBufferedScene()->receiveString("level-name", mLevelName);
  }
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