#include "SceneMenu.h"
#include "Game.h"

SceneMenu::SceneMenu() {}

SceneMenu::~SceneMenu() {
	if (mBackground != nullptr)
		delete mBackground;
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
}

void SceneMenu::updateGUI(int deltaTime) {
	Scene::updateGUI(deltaTime);

	mBackground->update(deltaTime);
}

void SceneMenu::renderGUI() {
	Scene::renderGUI();

	mBackground->render();
}