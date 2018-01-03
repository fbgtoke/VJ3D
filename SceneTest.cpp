#include "SceneTest.h"
#include "Game.h"

SceneTest::SceneTest()
  : Scene(Scene::SCENE_TEST), mLevel(nullptr), mLevelName(""), mPlayer(nullptr) {}

SceneTest::~SceneTest() {
  if (mLevel != nullptr) delete mLevel;
  if (mPlayer != nullptr) delete mPlayer;
}

void SceneTest::receiveString(const std::string& tag, const std::string str) {
  if (tag == "level-name")
    mLevelName = str;
}

void SceneTest::init() {
	Scene::init();

  if (mLevelName == "") {
    std::cout << "No level selected!" << std::endl;
    Game::instance().changeScene(Scene::SCENE_MENU);
    return;
  }

  mLevelInfo.setName(mLevelName);
  mLevel = LevelGenerator::generate("levels/" + mLevelName + "/");
  
  initPlayer();
  initCamera();
  mLightAngle = 0.f;

  Game::instance().getResource().setInt("score", 0);
  kScorePerTile = Game::instance().getResource().Int("scorePerTile");
  kScorePerSideWalk = Game::instance().getResource().Int("scorePerSideWalk");

  mFramebuffer.init();
  mDepthbuffer.init();
}

void SceneTest::update(int deltaTime) {
  Scene::update(deltaTime);

  if (Game::instance().getKeyPressed(27)) // Escape
    Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
  
  if (mLevel != nullptr) mLevel->update(deltaTime);

  if (mPlayer != nullptr) {
    mPlayer->update(deltaTime);
    mLevel->checkCollisions(mPlayer);

    checkPlayerInput();
    checkPlayerOutOfCamera();
    checkPlayerStandingTile();
    checkPlayerDead();
  }

  updateCamera(deltaTime);

  mLightAngle += (float)deltaTime * Game::instance().getResource().Float("sunSpeed");
}

void SceneTest::render() {
  Scene::render();

  renderShadowmap();
  renderFramebuffer();
  renderScene();
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

  if (mLevel != nullptr) {
    unsigned int width = mLevel->getTilemap().getWidth();
    float middleTile = (float)(width/2);
    mPlayer->setPositionInTiles(glm::vec3(middleTile, 0.f, 0.f));
  }
}

void SceneTest::renderShadowmap() {
  mTexProgram = Game::instance().getResource().shader("depth");
  mDepthbuffer.use();

  mTexProgram->use();
  glm::mat4 PM = getDepthProjectionMatrix();
  mTexProgram->setUniformMatrix4f("PM", PM);
  glm::mat4 VM = getDepthViewMatrix();
  mTexProgram->setUniformMatrix4f("VM", VM);
  
  if (mLevel != nullptr) mLevel->render();
  if (mPlayer != nullptr) mPlayer->render();
}

void SceneTest::renderFramebuffer() {
  mTexProgram = Game::instance().getResource().shader("level");
  mFramebuffer.use();

  mTexProgram->use();
  glm::mat4 PM = getProjectionMatrix();
  mTexProgram->setUniformMatrix4f("PM", PM);
  glm::mat4 VM = getViewMatrix();
  mTexProgram->setUniformMatrix4f("VM", VM);

  glm::mat4 biasMatrix = getDepthBiasMatrix();
  mTexProgram->setUniformMatrix4f("depthBiasMatrix", biasMatrix);
  glm::mat4 depthPM = getDepthProjectionMatrix();
  mTexProgram->setUniformMatrix4f("depthPM", depthPM);
  glm::mat4 depthVM = getDepthViewMatrix();
  mTexProgram->setUniformMatrix4f("depthVM", depthVM);

  glm::vec3 lightDirection = getLightDirection();
  glm::vec3 ambientColor = getAmbientColor();
  mTexProgram->setUniform3f("lightDir", lightDirection.x, lightDirection.y, lightDirection.z);
  mTexProgram->setUniform3f("ambientColor", ambientColor);
  
  mTexProgram->setUniform1i("shadow", 1);
  mDepthbuffer.getTexture()->use();

  if (mLevel != nullptr) mLevel->render();
  if (mPlayer != nullptr) mPlayer->render();
}

void SceneTest::renderScene() {
  mTexProgram = Game::instance().getResource().shader("post");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderGui();
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

  Obstacle* boat = getBoatAdjacentToPlayer(direction);
  Obstacle* stone = getStoneAdjacentToPlayer(direction);

  glm::vec3 targetTile = mPlayer->getPositionInTiles() + direction;
  glm::ivec2 targetTileInTilemap = mLevel->player2tilemap(targetTile);
  bool jumping = (direction != glm::vec3(0.f));
  bool outOfBounds = mLevel->getTilemap().outOfBounds(targetTileInTilemap);

  glm::vec3 targetPosition;
  if (jumping && !outOfBounds) {
    if (boat != nullptr) {
      mPlayer->moveTowardsBoat(boat);
    } else if (stone != nullptr) {
      targetPosition = stone->getTopCenter();
      targetPosition.y += mPlayer->getSize().y * 0.5f;
      mPlayer->moveTowards(targetPosition);
    } else {
      targetPosition = mPlayer->getPosition() + direction * TILE_SIZE;
      targetPosition.y = mPlayer->getSize().y * 0.5f;
      mPlayer->moveTowards(targetPosition);
    }

    if (direction == IN) addScore(kScorePerTile);
    else if (direction == OUT) removeScore(kScorePerTile);
    else removeScore(kScorePerSideWalk);
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

Obstacle* SceneTest::getBoatAdjacentToPlayer(const glm::vec3& direction) {
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

  if (boat != nullptr && boat->getType() == Obstacle::Boat)
    return boat;
  return nullptr;
}

Obstacle* SceneTest::getStoneAdjacentToPlayer(const glm::vec3& direction) {
  glm::vec3 adjacent[3][3];
  mPlayer->getAdjacentTiles(adjacent);

  Obstacle* stone = nullptr;
  if (direction == IN)
    stone = mLevel->getObstacleAtTile(adjacent[0][1]);
  else if (direction == OUT)
    stone = mLevel->getObstacleAtTile(adjacent[0][2]);

  if (stone != nullptr && stone->getType() == Obstacle::Stone)
    return stone;
  return nullptr;
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
  return glm::ortho(-1.f, 1.f, -1.f, 1.f, 0.001f, 20000.f);
}

glm::mat4 SceneTest::getViewMatrix() const {
  return glm::lookAt(OBS, VRP, UP);
}

glm::mat4 SceneTest::getDepthViewMatrix() const {
  glm::vec3 obs = 10 * TILE_SIZE * getLightDirection() * (-1.f);
  glm::vec3 vrp = glm::vec3(10.f, 0.f, 0.f);

  return glm::lookAt(obs, vrp, UP);
}

glm::mat4 SceneTest::getDepthBiasMatrix() const {
  return glm::mat4(
    0.5, 0.0, 0.0, 0.0,
    0.0, 0.5, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.5, 0.5, 0.5, 1.0
  );
}

void SceneTest::addScore(unsigned int score) {
  int currentScore = Game::instance().getResource().Int("score");
  Game::instance().getResource().setInt("score", currentScore + score);
}

void SceneTest::removeScore(unsigned int score) {
  int currentScore = Game::instance().getResource().Int("score");

  if (score > currentScore)
    Game::instance().getResource().setInt("score", 0);
  else
    Game::instance().getResource().setInt("score", currentScore - score);
}

void SceneTest::initGui() {
  Scene::initGui();

  mGui = Game::instance().getResource().layout("level.xml");
  mGui->getSprite("scene-frame")->flipY();
}

void SceneTest::updateGui() {
  Scene::updateGui();

  Text* scoreText = mGui->getText("score-text");
  if (scoreText != nullptr) {
    int score = Game::instance().getResource().Int("score");
    scoreText->setString(std::to_string(score));
  }
}

void SceneTest::renderGui() {
  mGui->getSprite("scene-frame")->setTexture(mFramebuffer.getTexture());

  Scene::renderGui();
}