#include "SceneTest.h"
#include "Game.h"

SceneTest::SceneTest()
  : Scene(Scene::SCENE_TEST), mLevelName(""), mPlayer(nullptr) {}

SceneTest::~SceneTest() {
  if (mPlayer != nullptr) delete mPlayer;
}

void SceneTest::receiveString(const std::string& tag, const std::string str) {
  if (tag == "level-name")
    mLevelName = str;
}

void SceneTest::init() {
	Scene::init();
  InputManager::enableTouchControls(true);

  mLevel.loadFromFile(mLevelName);
  
  initPlayer();
  mCamera.init();
  mCamera.follow(mPlayer);

  Game::instance().getResource().setInt("score", 0);
  kScorePerTile = Game::instance().getResource().Int("scorePerTile");
  kScorePerSideWalk = Game::instance().getResource().Int("scorePerSideWalk");

  mFramebuffer.init();
  mDepthbuffer.init();

  mSun.init();
  mSun.setCurrentTime((float)M_PI * 0.75f);
  mSun.setLevelSize(mLevel.getTilemap().getWidth(), mLevel.getTilemap().getHeight());

  Game::instance().setBackgroundMusic("ingame.ogg", 100.f);
  changeState(SceneTest::Ready);
}

void SceneTest::update(int deltaTime) {
  Scene::update(deltaTime);

  if (InputManager::getAction(InputManager::Close)) {
    Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
    Game::instance().getBufferedScene()->receiveString("level-name", mLevel.getName());
  }

  if (mLevelName == "") {
    std::cout << "No level selected!" << std::endl;
    Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
    return;
  }
  
  mLevel.update(deltaTime);

  switch (mState) {
  case SceneTest::Ready:
    if (mSoundEffects.empty())
      changeState(SceneTest::Go);
    break;
  case SceneTest::Go:
    if (mSoundEffects.empty())
      changeState(SceneTest::Playing);
    break;
  case SceneTest::Playing:
    mCamera.update(deltaTime);
    //mSun.update(deltaTime);

    if (mPlayer != nullptr) {
      mPlayer->update(deltaTime);
      mLevel.checkCollisions(mPlayer);

      checkPlayerInput();
      checkPlayerOutOfBounds();
      checkPlayerOutOfCamera();
      checkPlayerStandingTile();
    }

    if (mPlayer->isExploding())
      changeState(SceneTest::Dead);
    break;
  case SceneTest::Dead:
    if (InputManager::getAction(InputManager::Accept)) {
      Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
      Game::instance().getBufferedScene()->receiveString("level-name", mLevel.getName());
    }
    break;
  case SceneTest::Win:
    if (mSoundEffects.empty()) {
      Game::instance().changeScene(Scene::SCENE_LEVEL_SELECT);
      Game::instance().getBufferedScene()->receiveString("level-name", mLevel.getName());
    }
  }
}

void SceneTest::render() {
  Scene::render();

  renderShadowmap();
  renderFramebuffer();
  renderScene();
}

void SceneTest::initPlayer() {
  mPlayer = new Player();
  mPlayer->init();

  unsigned int width = mLevel.getTilemap().getWidth();
  float middleTile = (float)(width/2);
  mPlayer->setPositionInTiles(glm::vec3(middleTile, 0.f, 0.f));
}

void SceneTest::renderShadowmap() {
  mTexProgram = Game::instance().getResource().shader("depth");
  mDepthbuffer.use();

  mTexProgram->use();
  glm::mat4 PM = mSun.getProjectionMatrix();
  glm::mat4 VM = mSun.getViewMatrix();

  mTexProgram->setUniformMatrix4f("PM", PM);
  mTexProgram->setUniformMatrix4f("VM", VM);
  
  mLevel.render();
  if (mPlayer != nullptr) mPlayer->render();
}

void SceneTest::renderFramebuffer() {
  mTexProgram = Game::instance().getResource().shader("level");
  mFramebuffer.use();

  mTexProgram->use();
  glm::mat4 PM = mCamera.getProjectionMatrix();
  glm::mat4 VM = mCamera.getViewMatrix();

  glm::mat4 biasMatrix = mSun.getBiasMatrix();
  glm::mat4 depthPM = mSun.getProjectionMatrix();
  glm::mat4 depthVM = mSun.getViewMatrix();

  glm::vec3 lightDirection = mSun.getDirection();
  glm::vec3 lightColor = mSun.getColor();

  mTexProgram->setUniformMatrix4f("PM", PM);
  mTexProgram->setUniformMatrix4f("VM", VM);
  mTexProgram->setUniformMatrix4f("biasDepthMatrix", biasMatrix);
  mTexProgram->setUniformMatrix4f("depthPM", depthPM);
  mTexProgram->setUniformMatrix4f("depthVM", depthVM);
  mTexProgram->setUniform3f("lightDir", lightDirection);
  mTexProgram->setUniform3f("lightColor", lightColor);

  mTexProgram->setUniform1i("tex", 0);
  mTexProgram->setUniform1i("shadow", 1);
  mDepthbuffer.getTexture()->use();

  mLevel.render();
  if (mPlayer != nullptr) mPlayer->render();

  for (Particle* particle : mParticles)
    particle->render();
}

void SceneTest::renderScene() {
  mTexProgram = Game::instance().getResource().shader("post");
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  renderGui();
}

void SceneTest::checkPlayerInput() {
  Player::State state = mPlayer->getState();
  if (state != Player::Idle && state != Player::onBoat) return;
  
  glm::vec3 direction(0.f);
  if (InputManager::getAction(InputManager::Left) && state != Player::onBoat)
    direction = LEFT;
  else if (InputManager::getAction(InputManager::Right) && state != Player::onBoat)
    direction = RIGHT;
  else if (InputManager::getAction(InputManager::Up))
    direction = IN;
  else if (InputManager::getAction(InputManager::Down))
    direction = OUT;

  Obstacle* boat = getBoatAdjacentToPlayer(direction);
  Obstacle* stone = getStoneAdjacentToPlayer(direction);

  glm::vec3 targetTile = mPlayer->getPositionInTiles() + direction;
  targetTile.y = 0.f;
  glm::ivec2 targetTileInTilemap = mLevel.player2tilemap(targetTile);

  bool jumping = (direction != glm::vec3(0.f));
  bool outOfBounds = mLevel.getTilemap().outOfBounds(targetTileInTilemap);
  bool stumpInTarget = mLevel.obstacleOfTypeAtTile(Obstacle::Stump, targetTile);

  glm::vec3 targetPosition;
  if (jumping && !outOfBounds && !stumpInTarget) {
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

  if (jumping && stumpInTarget)
    mPlayer->playJumpSound();
}

void SceneTest::checkPlayerOutOfBounds() {
  glm::vec3 standingTile = mPlayer->getPositionInTiles();
  glm::ivec2 standingTileInTilemap = mLevel.player2tilemap(standingTile);

  if (mPlayer->isAlive() && mLevel.getTilemap().outOfBounds(standingTileInTilemap)) {
    mPlayer->explode();
    Game::instance().getScene()->playSoundEffect("deathOverrun0.ogg");
  }
}

void SceneTest::checkPlayerOutOfCamera() {
  if (!mPlayer->isAlive()) return;

  if (mPlayer->isAlive() && mCamera.outOfView(mPlayer)) {
    mPlayer->explode();
    Game::instance().getScene()->playSoundEffect("deathOverrun0.ogg");
  }
}

void SceneTest::checkPlayerStandingTile() {
  if (mPlayer != nullptr && mPlayer->getState() == Player::Idle) {
    glm::vec3 standingTile = mPlayer->getPositionInTiles();
    glm::ivec2 standingTileInTilemap = mLevel.player2tilemap(standingTile);
    Tile::Type tile = mLevel.getTilemap().getTile(standingTileInTilemap);

    if (tile == Tile::Water && !mLevel.obstacleOfTypeAtTile(Obstacle::Stone, standingTile))
      mPlayer->changeState(Player::Drowning);
    else if (tile == Tile::Goal)
      changeState(SceneTest::Win);
  }
}

Obstacle* SceneTest::getBoatAdjacentToPlayer(const glm::vec3& direction) {
  glm::vec3 adjacent[3][3];
  mPlayer->getAdjacentTiles(adjacent);

  Obstacle* boat = nullptr;
  if (direction == IN) {
    boat = mLevel.getObstacleAtTile(adjacent[0][0]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel.getObstacleAtTile(adjacent[0][1]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel.getObstacleAtTile(adjacent[0][2]);
  } else if (direction == OUT) {
    boat = mLevel.getObstacleAtTile(adjacent[2][0]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel.getObstacleAtTile(adjacent[2][1]);
    if (boat == nullptr || boat->getType() != Obstacle::Boat)
      boat = mLevel.getObstacleAtTile(adjacent[2][2]);
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
    stone = mLevel.getObstacleAtTile(adjacent[0][1]);
  else if (direction == OUT)
    stone = mLevel.getObstacleAtTile(adjacent[0][2]);

  if (stone != nullptr && stone->getType() == Obstacle::Stone)
    return stone;
  return nullptr;
}

void SceneTest::addScore(unsigned int score) {
  int currentScore = Game::instance().getResource().Int("score");
  Game::instance().getResource().setInt("score", currentScore + score);
}

void SceneTest::removeScore(unsigned int score) {
  unsigned int currentScore = (unsigned int)Game::instance().getResource().Int("score");

  if (score > currentScore)
    Game::instance().getResource().setInt("score", 0);
  else
    Game::instance().getResource().setInt("score", currentScore - score);
}

void SceneTest::initGui() {
  Scene::initGui();

  mGui = Game::instance().getResource().layout("level.xml");
  mGui->getSprite("scene-frame")->flipY();
  mGui->hideLayer(0);
  mGui->hideLayer(1);
}

void SceneTest::updateGui(int deltaTime) {
  Scene::updateGui(deltaTime);

  if (mState == SceneTest::Playing) {
    if (mPlayer->isDrunk()) {
      mGui->getSprite("scene-frame")->setShader(Game::instance().getResource().shader("drunk"));
      Game::instance().setBackgroundMusicPitch(0.5f);
    } else {
      mGui->getSprite("scene-frame")->setShader(Game::instance().getResource().shader("sprite"));
      Game::instance().setBackgroundMusicPitch(1.f);
    }
  }

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

void SceneTest::changeState(SceneTest::State state) {
  switch (state) {
  case SceneTest::Ready:
    Game::instance().getScene()->playSoundEffect("ready.ogg");
    mGui->showLayer(0);
    mGui->hideLayer(1); 
    mGui->hideLayer(2);
    break;
  case SceneTest::Go:
    Game::instance().getScene()->playSoundEffect("go.ogg");
    mGui->hideLayer(0);
    mGui->showLayer(1); 
    break;
  case SceneTest::Playing:
    mGui->hideLayer(1); 
    break;
  case SceneTest::Dead:
    mCamera.setMoving(false);
    Game::instance().setBackgroundMusicPitch(1.f);
    Game::instance().getScene()->playSoundEffect("youLose.ogg");
    mGui->showLayer(2);
    mGui->getSprite("scene-frame")->setShader(Game::instance().getResource().shader("post"));
    break;
  case SceneTest::Win:
    mGui->getSprite("scene-frame")->setShader(Game::instance().getResource().shader("sprite"));
    Game::instance().setBackgroundMusicPitch(1.f);
    Game::instance().getScene()->playSoundEffect("youWon.ogg");
    mLevel.updateHighscore();
    break;
  }

  mState = state;
}