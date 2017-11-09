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

void SceneTest::init() {
	Scene::init();

	float fov = M_PI / 3.0f;
	float width = SCREEN_WIDTH;
	float height = SCREEN_HEIGHT;
	mProjectionMatrix = glm::perspective(fov, width/height, 0.1f, 100.0f);

	glm::vec3 OBS = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 VRP = glm::vec3(0.0f, 0.0f, -5.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	mViewMatrix = glm::lookAt(OBS, VRP, up);

	mModel.init();
	mModel.loadFromFile("models/test3.txt");
	mModel.rotateX(M_PI / 4.0f);
	mModel.rotateY(M_PI / -4.0f);

	mTexButtonPlus.loadFromFile("images/button_plus.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonMinus.loadFromFile("images/button_minus.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonLeft.loadFromFile("images/button_left.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonRight.loadFromFile("images/button_right.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonUp.loadFromFile("images/button_up.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonDown.loadFromFile("images/button_down.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonRotateClockwise.loadFromFile("images/button_rot_clockwise.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonRotateCounterclockwise.loadFromFile("images/button_rot_counterclockwise.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonUpZ.loadFromFile("images/button_up_z.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mTexButtonDownZ.loadFromFile("images/button_down_z.png", TEXTURE_PIXEL_FORMAT_RGBA);

	mButtonLeft = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonLeft, &mGuiProgram);
	mButtonLeft->setOnClick([this](){ mCursor.move(glm::vec3(-1, 0, 0)); });
	mButtonLeft->setPosition(glm::vec2(64 * 0, SCREEN_HEIGHT - 64));

	mButtonRight = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonRight, &mGuiProgram);
	mButtonRight->setOnClick([this](){ mCursor.move(glm::vec3(1, 0, 0)); });
	mButtonRight->setPosition(glm::vec2(64 * 2, SCREEN_HEIGHT - 64));

	mButtonUp = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonUp, &mGuiProgram);
	mButtonUp->setOnClick([this](){ mCursor.move(glm::vec3(0, -1, 0)); });
	mButtonUp->setPosition(glm::vec2(64 * 1, SCREEN_HEIGHT - 64 * 2));

	mButtonDown = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonDown, &mGuiProgram);
	mButtonDown->setOnClick([this](){ mCursor.move(glm::vec3(0, 1, 0)); });
	mButtonDown->setPosition(glm::vec2(64 * 1, SCREEN_HEIGHT - 64 * 1));

	mButtonRotateClockwise = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonRotateClockwise, &mGuiProgram);
	mButtonRotateClockwise->setOnClick([this](){ mModel.rotateY(M_PI / -4.0f); });
	mButtonRotateClockwise->setPosition(glm::vec2(64 * 0, SCREEN_HEIGHT - 64 * 2));

	mButtonRotateCounterclockwise = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonRotateCounterclockwise, &mGuiProgram);
	mButtonRotateCounterclockwise->setOnClick([this](){ mModel.rotateY(M_PI / 4.0f); });
	mButtonRotateCounterclockwise->setPosition(glm::vec2(64 * 2, SCREEN_HEIGHT - 64 * 2));

	mButtonUpZ = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonUpZ, &mGuiProgram);
	mButtonUpZ->setOnClick([this](){ mCursor.move(glm::vec3(0, 0, 1)); });
	mButtonUpZ->setPosition(glm::vec2(64 * 0, SCREEN_HEIGHT - 64 * 3));

	mButtonDownZ = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonDownZ, &mGuiProgram);
	mButtonDownZ->setOnClick([this](){ mCursor.move(glm::vec3(0, 0, -1)); });
	mButtonDownZ->setPosition(glm::vec2(64 * 2, SCREEN_HEIGHT - 64 * 3));

	mButtonAlphaMinus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonMinus, &mGuiProgram);
	mButtonAlphaMinus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.w = 0.0f;
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonAlphaMinus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 5));

	mButtonAlphaPlus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonPlus, &mGuiProgram);
	mButtonAlphaPlus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.w = 1.0f;
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonAlphaPlus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 5));

	mButtonRedMinus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonMinus, &mGuiProgram);
	mButtonRedMinus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.r = std::max(color.r - 0.2f, 0.0f);
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonRedMinus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 4));

	mButtonRedPlus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonPlus, &mGuiProgram);
	mButtonRedPlus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.r = std::min(color.r + 0.2f, 1.0f);
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonRedPlus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 4));

	mButtonGreenMinus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonMinus, &mGuiProgram);
	mButtonGreenMinus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.g = std::max(color.g - 0.2f, 0.0f);
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonGreenMinus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 3));

	mButtonGreenPlus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonPlus, &mGuiProgram);
	mButtonGreenPlus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.g = std::min(color.g + 0.2f, 1.0f);
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonGreenPlus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 3));

	mButtonBlueMinus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonMinus, &mGuiProgram);
	mButtonBlueMinus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.b = std::max(color.b - 0.2f, 0.0f);
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonBlueMinus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 2, SCREEN_HEIGHT - 64 * 2));

	mButtonBluePlus = new Button(glm::vec2(32, 32), glm::vec2(1.0f, 1.0f), &mTexButtonPlus, &mGuiProgram);
	mButtonBluePlus->setOnClick([this](){
		glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
		glm::vec4 color;

		color = mModel.getCubeColor(cursorIndex);
		color.b = std::min(color.b + 0.2f, 1.0f);
		mModel.setCubeColor(cursorIndex, color);
	});
	mButtonBluePlus->setPosition(glm::vec2(SCREEN_WIDTH - 64 * 1, SCREEN_HEIGHT - 64 * 2));

	mCursor.init();
}

void SceneTest::update(int deltaTime) {
	Scene::update(deltaTime);
	mModel.update(deltaTime);

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

	if (Game::instance().getKeyPressed('w')) mButtonUp->click();
	if (Game::instance().getKeyPressed('a')) mButtonLeft->click();
	if (Game::instance().getKeyPressed('s')) mButtonDown->click();
	if (Game::instance().getKeyPressed('d')) mButtonRight->click();

	if (Game::instance().getKeyPressed('q')) mButtonRotateClockwise->click();
	if (Game::instance().getKeyPressed('e')) mButtonRotateCounterclockwise->click();

	if (Game::instance().getKeyPressed('1')) mButtonUpZ->click();
	if (Game::instance().getKeyPressed('3')) mButtonDownZ->click();
}

void SceneTest::render() {
	Scene::render();
	mTexProgram.setUniform3f("lightDir", kLightDirection.x, kLightDirection.y, kLightDirection.z);
	mTexProgram.setUniform1f("ambientColor", kAmbientLight);
	mModel.render();
	mCursor.render();
	glDisable(GL_CULL_FACE);

	glm::mat4 auxProject = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	glm::mat4 auxView = glm::mat4(1.0f);
	glm::mat4 auxModel = glm::mat4(1.0f);

	mGuiProgram.use();
	mGuiProgram.setUniformMatrix4f("PM", auxProject);
	mGuiProgram.setUniformMatrix4f("VM", auxView);
	mGuiProgram.setUniformMatrix4f("modelview", auxModel);
	mGuiProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	mGuiProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

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