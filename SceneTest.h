#ifndef _SCENE_TEST_INCLUDE
#define _SCENE_TEST_INCLUDE

#include "Scene.h"
#include "Model.h"

#include "Cursor.h"

#include "Button.h"
#include "Texture.h"

class SceneTest : public Scene {
public:
	SceneTest();
	~SceneTest() final;

	void init() final;
	void update(int deltaTime) final;
	void render() final;

private:
	static const glm::vec3 kLightDirection;
	static const float kAmbientLight;
	Model mModel;

	Texture mTexButtonPlus;
	Button* mButtonPlus;

	Texture mTexButtonMinus;
	Button* mButtonMinus;

	Cursor mCursor;
};

#endif // _SCENE_TEST_INCLUDE