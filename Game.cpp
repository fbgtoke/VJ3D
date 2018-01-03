#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Game::init() {
	bPlay = true;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);

	mResourceManager.loadConstants();

	mBackgroundMusic = nullptr;

	scene = Scene::create(Scene::SCENE_MENU);
	scene->init();

	bufferedScene = nullptr;
}

bool Game::update(int deltaTime) {
	scene->update(deltaTime);

	if (bufferedScene != nullptr) {
		if (scene != nullptr) {
			delete scene;
		}

		scene = bufferedScene;
		scene->init();

		bufferedScene = nullptr;
	}

	return bPlay;
}

void Game::render() {
	scene->render();
}

void Game::keyPressed(int key) {
	keys[key] = true;
}

void Game::keyReleased(int key) {
	keys[key] = false;
}

void Game::specialKeyPressed(int key) {
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key) {
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y) {
	mMousePosition = glm::ivec2(x, y);
}

void Game::mousePress(int button) {
	if (button == GLUT_LEFT_BUTTON)
		mMouseButton = true;
}

void Game::mouseRelease(int button) {
	if (button == GLUT_LEFT_BUTTON)
		mMouseButton = false;
}

bool Game::getKey(int key) const {
	return keys[key];
}

bool Game::getSpecialKey(int key) const {
	return specialKeys[key];
}

void Game::scanKeys() {
	for (int i = 0; i < 256; ++i) {
		mKeysPrevious[i] = mKeysCurrent[i];
		mKeysCurrent[i] = getKey(i);

		mSpecialKeysPrevious[i] = mSpecialKeysCurrent[i];
		mSpecialKeysCurrent[i] = getSpecialKey(i);
	}

	mMouseButtonPrevious = mMouseButtonCurrent;
	mMouseButtonCurrent = mMouseButton;
}

bool Game::getKeyPressed(int key) const {
	return !mKeysPrevious[key] && mKeysCurrent[key];
}

bool Game::getKeyReleased(int key) const {
	return mKeysPrevious[key] && !mKeysCurrent[key];
}

bool Game::getSpecialKeyPressed(int key) const {
	return !mSpecialKeysPrevious[key] && mSpecialKeysCurrent[key];
}

bool Game::getSpecialKeyReleased(int key) const {
	return mSpecialKeysPrevious[key] && !mSpecialKeysCurrent[key];
}

glm::ivec2 Game::getMousePosition() const {
	return mMousePosition;
}

bool Game::getMouseButton() const {
	return mMouseButton;
}

bool Game::getMouseButtonPressed() const {
	return !mMouseButtonPrevious && mMouseButtonCurrent;
}

bool Game::getMouseButtonReleased() const {
	return mMouseButtonPrevious && !mMouseButtonCurrent;
}

void Game::resize(int width, int height) {
	if (width != SCREEN_WIDTH || height != SCREEN_HEIGHT)
		glutReshapeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::changeScene(Scene::SceneType type) {
	if (bufferedScene != nullptr)
		delete bufferedScene;

	bufferedScene = Scene::create(type);
}

Scene* Game::getScene() { return scene; }
Scene* Game::getBufferedScene() { return bufferedScene; }

void Game::stop() { bPlay = false; }

ResourceManager& Game::getResource() { return mResourceManager; }

void Game::setBackgroundMusic(const std::string& name, float volume) {
	sf::Music* music = getResource().music(name);
	if (music != nullptr && music != mBackgroundMusic) {
		if (mBackgroundMusic != nullptr)
			mBackgroundMusic->stop();

		mBackgroundMusic = music;
		mBackgroundMusic->play();
		mBackgroundMusic->setVolume(volume);
		mBackgroundMusic->setLoop(true);
	}
}