#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include "utils.h"

class Scene {
public:
	Scene();
	virtual ~Scene();

	void init();
	void update(int deltaTime);
	void render();

	enum SceneType {
		SCENE_TEST,
		SCENE_MENU,
		SCENE_PLAY
	};
	static Scene* create(SceneType type);

	virtual void initShaders();

protected:
	float mCurrentTime;

	ShaderProgram *mTexProgram, *mGuiProgram;
	glm::mat4 mProjectionMatrix, mViewMatrix;
	glm::mat4 mProjectionMatrixGUI, mViewMatrixGUI;

	glm::vec3 OBS, VRP;

	static const glm::vec3 kLightDirection;
	static const float kAmbientLight;

	virtual void initScene();
	virtual void updateScene(int deltaTime);
	virtual void renderScene();

	virtual void initGUI();
	virtual void updateGUI(int deltaTime);
	virtual void renderGUI();
};

#endif // _SCENE_INCLUDE