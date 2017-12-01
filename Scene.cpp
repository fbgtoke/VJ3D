#include "Scene.h"
#include "SceneTest.h"

#include "Game.h"

Scene::Scene() {}

Scene::~Scene() {}

Scene* Scene::create(SceneType type) {
	switch(type) {
	default:
		return new SceneTest();
	}
}

void Scene::init() {
	initShaders();
	initScene();
	initGUI();
	mCurrentTime = 0.0f;
}

void Scene::update(int deltaTime) {
	Game::instance().scanKeys();
	mCurrentTime += deltaTime;

	updateScene(deltaTime);
	updateGUI(deltaTime);
}

void Scene::render() {
	renderScene();
	renderGUI();
}

void Scene::initShaders() {
	mTexProgram = Game::instance().getResource().shader("simple");
	mGuiProgram = Game::instance().getResource().shader("texture");
}

void Scene::initScene() {
	mProjectionMatrix = glm::mat4(1.0f);
	mViewMatrix = glm::mat4(1.0f);
}

void Scene::updateScene(int deltaTime) {}

void Scene::renderScene() {
	mTexProgram->use();
	mTexProgram->setUniformMatrix4f("PM", mProjectionMatrix);
	mTexProgram->setUniformMatrix4f("VM", mViewMatrix);
}

void Scene::initGUI() {
	mProjectionMatrixGUI = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	mViewMatrixGUI = glm::mat4(1.0f);
}

void Scene::updateGUI(int deltaTime) {}

void Scene::renderGUI() {
	mGuiProgram->use();
	mGuiProgram->setUniformMatrix4f("PM", mProjectionMatrixGUI);
	mGuiProgram->setUniformMatrix4f("VM", mViewMatrixGUI);
	mGuiProgram->setUniform2f("texCoordDispl", 0.f, 0.f);
	mGuiProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
}