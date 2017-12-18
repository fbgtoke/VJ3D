#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include "utils.h"
#include "Model.h"

class Scene {
public:
	Scene();
	virtual ~Scene();

	void init();
	void update(int deltaTime);
	void render();

	static glm::vec3 getLightDirection();
	static float getAmbientLight();

	glm::mat4 getProjectionMatrix() const;
	glm::mat4 getViewMatrix() const;

	enum SceneType {
		SCENE_TEST,
		SCENE_MENU,
		SCENE_LEVEL_SELECT,
		SCENE_PLAY,
		SCENE_WIN,
		SCENE_DEAD
	};
	static Scene* create(SceneType type);

	virtual void initShaders();

	void playSoundEffect(const std::string& name);

	enum ModelFlags : unsigned char {
		RenderNone  = 0x01,
		RenderFirst = 0x02,
		RenderLast  = 0x04,
		UpdateNone  = 0x08,
		UpdateFirst = 0x0A,
		UpdateLast  = 0x0C
	};
	void addModel(Model* model, unsigned char render = RenderLast | UpdateLast);
  void removeModel(Model* model);

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

	void checkSoundEffects();
	std::list<sf::Sound*> mSoundEffects;

	std::set<Model*> mModels;
	std::list<Model*> mUpdateList;
	std::list<Model*> mRenderList;
};

#endif // _SCENE_INCLUDE