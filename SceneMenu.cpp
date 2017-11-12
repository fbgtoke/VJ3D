#include "SceneMenu.h"
#include "Game.h"

SceneMenu::SceneMenu()
	: text(mTextProgram) {}

SceneMenu::~SceneMenu() {
	if (mBackground != nullptr)
		delete mBackground;

	for (Button* button : mButtons) {
		if (button != nullptr) {
			delete button;
			button = nullptr;
		}
	}

	mButtons.clear();
}

void SceneMenu::initShaders() {
	Scene::initShaders();

	Shader vShader, fShader;

	/* Init shaders for text */
	vShader.initFromFile(VERTEX_SHADER, "shaders/text.vert");
	if(!vShader.isCompiled())
	{
		cout << "Text Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/text.frag");
	if(!fShader.isCompiled())
	{
		cout << "Text Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	mTextProgram.init();
	mTextProgram.addShader(vShader);
	mTextProgram.addShader(fShader);
	mTextProgram.link();
	if(!mTextProgram.isLinked())
	{
		cout << "Text Shader Linking Error" << endl;
		cout << "" << mTextProgram.log() << endl << endl;
	}
	mTextProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void SceneMenu::initScene() {
	Scene::initScene();

	Game::instance().getResource()->setBackgroundMusic("menu.ogg");
}

void SceneMenu::updateScene(int deltaTime) {}
void SceneMenu::renderScene() {}

void SceneMenu::initGUI() {
	Scene::initGUI();

	mBackground = Sprite::createSprite(
		glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT),
		glm::vec2(1.0f, 1.0f),
		Game::instance().getResource()->getTexture("menu_background.png"),
		&mGuiProgram
	);
	mBackground->setNumberAnimations(1);
	mBackground->setAnimationSpeed(0, 0);
	mBackground->addKeyframe(0, glm::vec2(0.0f, 0.0f));

	text.init("fonts/font.ttf");
	text.setString("TEST");
	text.setPosition(glm::vec2(SCREEN_WIDTH/2 - 64, 0));
	text.setSize(64);

	mButtons.push_back(new Button(
		glm::vec2(256, 64),
		Game::instance().getResource()->getTexture("button_large.png"),
		&mGuiProgram,
		glm::vec2(SCREEN_WIDTH/2 - 128, 128),
		[this](){ Game::instance().changeScene(Scene::SCENE_EDITOR); }
	));
}

void SceneMenu::updateGUI(int deltaTime) {
	Scene::updateGUI(deltaTime);

	mBackground->update(deltaTime);
	for (Button* button : mButtons)
		if (button != nullptr)
			button->update(deltaTime);
}

void SceneMenu::renderGUI() {
	mTextProgram.use();
	mTextProgram.setUniformMatrix4f("PM", mProjectionMatrixGUI);
	mTextProgram.setUniformMatrix4f("VM", mViewMatrixGUI);
	text.render();

	Scene::renderGUI();
	for (Button* button : mButtons)
		if (button != nullptr)
			button->render();
	mBackground->render();
}