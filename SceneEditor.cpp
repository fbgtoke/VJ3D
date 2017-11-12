#include "SceneEditor.h"
#include "Game.h"

const glm::vec3 SceneEditor::kLightDirection = glm::normalize(glm::vec3(0.0, -4.0, -1.0));
const float SceneEditor::kAmbientLight = 0.4f;

SceneEditor::SceneEditor()
	: mModel(mTexProgram), mAxis(mTexProgram),
	mCursor(mModel, mTexProgram) {}

SceneEditor::~SceneEditor() {
	for (Button* button : mButtons) {
		if (button != nullptr) {
			delete button;
			button = nullptr;
		}
	}

	mButtons.clear();
}

void SceneEditor::initScene() {
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
	mModel.loadFromFile("models/test4.txt");
	mModel.rotateY(M_PI / -4.0f);

	mAxis.init();
	mAxis.loadFromFile("models/axis.txt");
	mAxis.rotateY(M_PI / -4.0f);
	mAxis.setPosition(glm::vec3(-4.0f, -4.0f, 0.0f));
	mAxis.scale(0.3f);

	mCursor.init();
}

void SceneEditor::updateScene(int deltaTime) {
	Scene::updateScene(deltaTime);

	mModel.update(deltaTime);
	mAxis.update(deltaTime);
	mCursor.update(deltaTime);

	if (Game::instance().getKeyPressed('q')) {
		mModel.rotateY(M_PI / -8.0f);
		mAxis.rotateY(M_PI / -8.0f);
	}

	if (Game::instance().getKeyPressed('e')) {
		mModel.rotateY(M_PI /  8.0f);
		mAxis.rotateY(M_PI /  8.0f);
	}

	if (Game::instance().getKeyPressed('a')) mCursor.move(glm::vec3(-1, 0, 0));
	if (Game::instance().getKeyPressed('d')) mCursor.move(glm::vec3( 1, 0, 0));

	if (Game::instance().getKeyPressed('w')) mCursor.move(glm::vec3(0, -1, 0));
	if (Game::instance().getKeyPressed('s')) mCursor.move(glm::vec3(0,  1, 0));

	if (Game::instance().getKeyPressed('1')) mCursor.move(glm::vec3(0, 0,  1));
	if (Game::instance().getKeyPressed('3')) mCursor.move(glm::vec3(0, 0, -1));

	if (Game::instance().getKeyPressed(' ')) mCursor.toggleCube();

	if (Game::instance().getKeyPressed('+')) mModel.move(glm::vec3(0.0f, 0.0f,  0.2f));
	if (Game::instance().getKeyPressed('-')) mModel.move(glm::vec3(0.0f, 0.0f, -0.2f));

	if (Game::instance().getKeyPressed(27)) // Escape
		Game::instance().changeScene(SCENE_MENU);
}

void SceneEditor::renderScene() {
	Scene::renderScene();

	glEnable(GL_CULL_FACE);
	mTexProgram.setUniform3f("lightDir", kLightDirection.x, kLightDirection.y, kLightDirection.z);
	mTexProgram.setUniform1f("ambientColor", kAmbientLight);
	mModel.render();
	mAxis.render();
	mCursor.render();
	glDisable(GL_CULL_FACE);
}

void SceneEditor::initGUI() {
	Scene::initGUI();

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_left.png"),
		&mGuiProgram,
		glm::vec2(32 * 1, SCREEN_HEIGHT - 32 * 2),
		[this](){ mCursor.move(glm::vec3(-1, 0, 0)); }
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_right.png"),
		&mGuiProgram,
		glm::vec2(32 * 5, SCREEN_HEIGHT - 32 * 2),
		[this](){ mCursor.move(glm::vec3(1, 0, 0)); }
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_up.png"),
		&mGuiProgram,
		glm::vec2(32 * 3, SCREEN_HEIGHT - 32 * 4),
		[this](){ mCursor.move(glm::vec3(0, -1, 0)); }
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_down.png"),
		&mGuiProgram,
		glm::vec2(32 * 3, SCREEN_HEIGHT - 32 * 2),
		[this](){ mCursor.move(glm::vec3(0, 1, 0)); }
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_rot_clockwise.png"),
		&mGuiProgram,
		glm::vec2(32 * 1, SCREEN_HEIGHT - 32 * 4),
		[this](){
			mModel.rotateY(M_PI / -4.0f);
			mAxis.rotateY(M_PI / -4.0f);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_rot_counterclockwise.png"),
		&mGuiProgram,
		glm::vec2(32 * 5, SCREEN_HEIGHT - 32 * 4),
		[this](){
			mModel.rotateY(M_PI / 4.0f);
			mAxis.rotateY(M_PI / 4.0f);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_up_z.png"),
		&mGuiProgram,
		glm::vec2(32 * 1, SCREEN_HEIGHT - 32 * 6),
		[this](){ mCursor.move(glm::vec3(0, 0, 1)); }
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_down_z.png"),
		&mGuiProgram,
		glm::vec2(32 * 5, SCREEN_HEIGHT - 32 * 6),
		[this](){ mCursor.move(glm::vec3(0, 0, -1)); }
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_minus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 32 * 4, SCREEN_HEIGHT - 32 * 8),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.r = std::max(color.r - 0.2f, 0.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_plus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 32 * 2, SCREEN_HEIGHT - 32 * 8),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.r = std::min(color.r + 0.2f, 1.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_minus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 32 * 4, SCREEN_HEIGHT - 32 * 6),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.g = std::max(color.g - 0.2f, 0.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_plus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 32 * 2, SCREEN_HEIGHT - 32 * 6),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.g = std::min(color.g + 0.2f, 1.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_minus.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 32 * 4, SCREEN_HEIGHT - 32 * 4),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.b = std::max(color.b - 0.2f, 0.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(32, 32),
		Game::instance().getResource()->getTexture("button_plus.png"), 
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 32 * 2, SCREEN_HEIGHT - 32 * 4),
		[this](){
			glm::ivec3 cursorIndex = (glm::ivec3) mCursor.getCurrentIndex();
			glm::vec4 color;

			color = mModel.getCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z);
			color.b = std::min(color.b + 0.2f, 1.0f);
			mModel.setCubeColor(cursorIndex.x, cursorIndex.y, cursorIndex.z, color);
		}
	));

	mButtons.push_back(new Button(
		glm::vec2(64, 32),
		Game::instance().getResource()->getTexture("button_save.png"), 
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH - 32 * 4, SCREEN_HEIGHT - 32 * 2),
		[this](){ mModel.saveToFile("models/test4.txt"); }
	));
}

void SceneEditor::updateGUI(int deltaTime) {
	Scene::updateGUI(deltaTime);

	for (Button* button : mButtons)
		if (button != nullptr)
			button->update(deltaTime);
}

void SceneEditor::renderGUI() {
	Scene::renderGUI();

	for (Button* button : mButtons)
		if (button != nullptr)
			button->render();
}