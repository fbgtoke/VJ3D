#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include "utils.h"

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();

	enum SceneType {
		SCENE_TEST
	};
	static Scene* create(SceneType type);

	void initShaders();

protected:
	float mCurrentTime;

	ShaderProgram mTexProgram, mGuiProgram;
	glm::mat4 mProjectionMatrix;
	glm::mat4 mViewMatrix;
};

#endif // _SCENE_INCLUDE