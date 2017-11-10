#include "SceneTest.h"
#include "Game.h"

const glm::vec3 SceneTest::kLightDirection = glm::normalize(glm::vec3(0.0, -4.0, -1.0));
const float SceneTest::kAmbientLight = 0.4f;

SceneTest::SceneTest()
	: mModel(mTexProgram), mCursor(mModel, mTexProgram) {}

SceneTest::~SceneTest() {
	if (mButtonLeft != nullptr) delete mButtonLeft;
	if (mButtonRight != nullptr) delete mButtonRight;
	if (mButtonUp != nullptr) delete mButtonUp;
	if (mButtonDown != nullptr) delete mButtonDown;
	if (mButtonRotateClockwise != nullptr) delete mButtonRotateClockwise;
	if (mButtonRotateCounterclockwise != nullptr) delete mButtonRotateCounterclockwise;
	if (mButtonUpZ != nullptr) delete mButtonUpZ;
	if (mButtonDownZ != nullptr) delete mButtonDownZ;
	if (mButtonAlphaPlus != nullptr) delete mButtonAlphaPlus;
	if (mButtonAlphaMinus != nullptr) delete mButtonAlphaMinus;
	if (mButtonRedPlus != nullptr) delete mButtonRedPlus;
	if (mButtonRedMinus != nullptr) delete mButtonRedMinus;
	if (mButtonGreenPlus != nullptr) delete mButtonGreenPlus;
	if (mButtonGreenMinus != nullptr) delete mButtonGreenMinus;
	if (mButtonBluePlus != nullptr) delete mButtonBluePlus;
	if (mButtonBlueMinus != nullptr) delete mButtonBlueMinus;
}

void SceneTest::initScene() {
	Scene::initScene();

	float fov = M_PI / 3.0f;
	float width = SCREEN_WIDTH;
	float height = SCREEN_HEIGHT;
	mProjectionMatrix = glm::perspective(fov, width/height, 0.1f, 100.0f);

	glm::vec3 OBS = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 VRP = glm::vec3(0.0f, 0.0f, -5.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	mViewMatrix = glm::lookAt(OBS, VRP, up);

	mModel.init();
	mModel.loadFromFile("models/test.txt");
	mModel.rotateX(M_PI / 4.0f);
	mModel.rotateY(M_PI / -4.0f);

	mCursor.init();
}

void SceneTest::updateScene(int deltaTime) {
	Scene::updateScene(deltaTime);

	mModel.update(deltaTime);

	if (Game::instance().getKeyPressed('w')) mButtonUp->click();
	if (Game::instance().getKeyPressed('a')) mButtonLeft->click();
	if (Game::instance().getKeyPressed('s')) mButtonDown->click();
	if (Game::instance().getKeyPressed('d')) mButtonRight->click();

	if (Game::instance().getKeyPressed('q')) mButtonRotateClockwise->click();
	if (Game::instance().getKeyPressed('e')) mButtonRotateCounterclockwise->click();

	if (Game::instance().getKeyPressed('1')) mButtonUpZ->click();
	if (Game::instance().getKeyPressed('3')) mButtonDownZ->click();
}

void SceneTest::renderScene() {
	Scene::renderScene();

	glEnable(GL_CULL_FACE);
	mTexProgram.setUniform3f("lightDir", kLightDirection.x, kLightDirection.y, kLightDirection.z);
	mTexProgram.setUniform1f("ambientColor", kAmbientLight);
	mModel.render();
	mCursor.render();
	glDisable(GL_CULL_FACE);
}

void SceneTest::initGUI() {
	Scene::initGUI();
	
	mButtonLeft = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_left.png"),
		&mGuiProgram,
		glm::vec2(64 * 0, SCREEN_HEIGHT - 64),
		[this](){ mCursor.move(glm::vec3(-1, 0, 0)); }
	);

	mButtonRight = new Button(
		glm::vec2(32, 32), 
		glm::vec2(1.0f, 1.0f), 
		Game::instance().getResource()->getTexture("button_right.png"),
		&mGuiProgram,
		glm::vec2(64 * 2, SCREEN_HEIGHT - 64),
		[this](){ mCursor.move(glm::vec3(1, 0, 0)); }
	);

	mButtonUp = new Button(
		glm::vec2(32, 32), 
		glm::vec2(1.0f, 1.0f), 
		Game::instance().getResource()->getTexture("button_up.png"),
		&mGuiProgram,
		glm::vec2(64 * 1, SCREEN_HEIGHT - 64 * 2),
		[this](){ mCursor.move(glm::vec3(0, -1, 0)); }
	);

	mButtonDown = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_up.png"),
		&mGuiProgram,
		glm::vec2(64 * 1, SCREEN_HEIGHT - 64 * 1),
		[this](){ mCursor.move(glm::vec3(0, 1, 0)); }
	);

	mButtonRotateClockwise = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_rot_clockwise.png"),
		&mGuiProgram,
		glm::vec2(64 * 0, SCREEN_HEIGHT - 64 * 2),
		[this](){ mModel.rotateY(M_PI / -4.0f); }
	);

	mButtonRotateCounterclockwise = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_rot_counterclockwise.png"),
		&mGuiProgram,
		glm::vec2(64 * 2, SCREEN_HEIGHT - 64 * 2),
		[this](){ mModel.rotateY(M_PI / 4.0f); }
	);

	mButtonUpZ = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_up_z.png"),
		&mGuiProgram,
		glm::vec2(64 * 0, SCREEN_HEIGHT - 64 * 3),
		[this](){ mCursor.move(glm::vec3(0, 0, 1)); }
	);

	mButtonDownZ = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_down_z.png"),
		&mGuiProgram,
		glm::vec2(64 * 2, SCREEN_HEIGHT - 64 * 3),
		[this](){ mCursor.move(glm::vec3(0, 0, -1)); }
	);

	mButtonAlphaMinus = new Button(
		glm::vec2(32, 32), 
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_minus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 5),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.w = 0.0f;
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);

	mButtonAlphaPlus = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_plus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 5),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.w = 1.0f;
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);

	mButtonRedMinus = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_minus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 4),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.r = std::max(color.r - 0.2f, 0.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);

	mButtonRedPlus = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_plus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 4),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.r = std::min(color.r + 0.2f, 1.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);

	mButtonGreenMinus = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_minus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 3),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.g = std::max(color.g - 0.2f, 0.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);

	mButtonGreenPlus = new Button(
		glm::vec2(32, 32),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_plus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 3),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.g = std::min(color.g + 0.2f, 1.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);

	mButtonBlueMinus = new Button(
		glm::vec2(32, 32), 
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("button_minus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 2),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.b = std::max(color.b - 0.2f, 0.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);

	mButtonBluePlus = new Button(
		glm::vec2(32, 32), 
		glm::vec2(1.0f, 1.0f), 
		Game::instance().getResource()->getTexture("button_plus.png"), 
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 2),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.b = std::min(color.b + 0.2f, 1.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	);
}

void SceneTest::updateGUI(int deltaTime) {
	Scene::updateGUI(deltaTime);

	mButtonLeft->update(deltaTime);
	mButtonRight->update(deltaTime);
	mButtonUp->update(deltaTime);
	mButtonDown->update(deltaTime);
	mButtonRotateClockwise->update(deltaTime);
	mButtonRotateCounterclockwise->update(deltaTime);
	mButtonUpZ->update(deltaTime);
	mButtonDownZ->update(deltaTime);
	mButtonAlphaMinus->update(deltaTime);
	mButtonAlphaPlus->update(deltaTime);
	mButtonRedMinus->update(deltaTime);
	mButtonRedPlus->update(deltaTime);
	mButtonGreenMinus->update(deltaTime);
	mButtonGreenPlus->update(deltaTime);
	mButtonBlueMinus->update(deltaTime);
	mButtonBluePlus->update(deltaTime);
}

void SceneTest::renderGUI() {
	Scene::renderGUI();

	mButtonLeft->render();
	mButtonRight->render();
	mButtonUp->render();
	mButtonDown->render();
	mButtonUpZ->render();
	mButtonDownZ->render();
	mButtonRotateClockwise->render();
	mButtonRotateCounterclockwise->render();
	mButtonAlphaMinus->render();
	mButtonAlphaPlus->render();
	mButtonRedMinus->render();
	mButtonRedPlus->render();
	mButtonGreenMinus->render();
	mButtonGreenPlus->render();
	mButtonBlueMinus->render();
	mButtonBluePlus->render();
}