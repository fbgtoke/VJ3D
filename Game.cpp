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
	InputManager::init();
}

bool Game::update(int deltaTime) {
	InputManager::update();
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
    mBackgroundMusic->setVolume(100.f);
    mBackgroundMusic->setLoop(true);
	}
}
