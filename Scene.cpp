#include "Scene.h"
#include "SceneTest.h"

#include "Game.h"

Scene::Scene() {}

Scene::~Scene() {}

Scene* Scene::create(SceneType type) {
	switch(type) {
	case SCENE_TEST:
		return new SceneTest();
	default:
		return nullptr;
	}
}

void Scene::init() {
	initShaders();
	
	mProjectionMatrix = glm::mat4(1.0f);
	mViewMatrix = glm::mat4(1.0f);
	mCurrentTime = 0.0f;
}

void Scene::update(int deltaTime) {
	Game::instance().scanKeys();
	mCurrentTime += deltaTime;
}

void Scene::render() {
	glEnable(GL_CULL_FACE);
	mTexProgram.use();
	mTexProgram.setUniformMatrix4f("PM", mProjectionMatrix);
	mTexProgram.setUniformMatrix4f("VM", mViewMatrix);
}

void Scene::initShaders() {
	Shader vShader, fShader;

	/* Init shaders for 3D models */
	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if(!vShader.isCompiled())
	{
		cout << "3D Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if(!fShader.isCompiled())
	{
		cout << "3D Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	mTexProgram.init();
	mTexProgram.addShader(vShader);
	mTexProgram.addShader(fShader);
	mTexProgram.link();
	if(!mTexProgram.isLinked())
	{
		cout << "3D Shader Linking Error" << endl;
		cout << "" << mTexProgram.log() << endl << endl;
	}
	mTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	/* Init shaders for GUI */
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "GUI Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "GUI Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	mGuiProgram.init();
	mGuiProgram.addShader(vShader);
	mGuiProgram.addShader(fShader);
	mGuiProgram.link();
	if(!mGuiProgram.isLinked())
	{
		cout << "GUI Shader Linking Error" << endl;
		cout << "" << mGuiProgram.log() << endl << endl;
	}
	mGuiProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}