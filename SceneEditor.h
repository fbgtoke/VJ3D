#ifndef _SCENE_EDITOR_INCLUDE
#define _SCENE_EDITOR_INCLUDE

#include "Scene.h"
#include "Model.h"

#include "Cursor.h"

#include "Button.h"
#include "Texture.h"

class SceneEditor : public Scene {
public:
	SceneEditor();
	~SceneEditor() override;

private:
	static const glm::vec3 kLightDirection;
	static const float kAmbientLight;
	Model mModel;
	Model mAxis;

	std::list<Button*> mButtons;

	Cursor mCursor;

	void initScene() override;
	void updateScene(int deltaTime) override;
	void renderScene() override;

	void initGUI() override;
	void updateGUI(int deltaTime) override;
	void renderGUI() override;
};

#endif // _SCENE_TEST_INCLUDE