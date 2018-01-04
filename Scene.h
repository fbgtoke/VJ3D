#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include "utils.h"
#include "Model.h"
#include "Particle.h"
#include "Gui.h"

class Scene {
public:
	enum SceneType {
		SCENE_TEST,
		SCENE_MENU,
		SCENE_LEVEL_SELECT,
		SCENE_PLAY,
		SCENE_WIN,
		SCENE_DEAD,
		SCENE_HELP,
		SCENE_CREDITS
	};
	
	Scene(Scene::SceneType type);
	virtual ~Scene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();

	virtual void receiveString(const std::string& tag, const std::string str);

	ShaderProgram* getShader();

	virtual glm::vec3 getLightDirection() const;
	virtual glm::vec3 getAmbientColor() const;

	virtual glm::mat4 getProjectionMatrix() const;
	virtual glm::mat4 getViewMatrix() const;

	static Scene* create(SceneType type);
	Scene::SceneType getType() const;

	void playSoundEffect(const std::string& name);
	void addParticle(Particle* particle);

	virtual bool outOfCamera(Model* model) const;

protected:
	const Scene::SceneType mType;
	float mCurrentTime;

	ShaderProgram *mTexProgram;
	glm::mat4 mProjectionMatrix, mViewMatrix;

	glm::vec3 OBS, VRP;

	glm::vec3 kLightDirection;
	glm::vec3 kAmbientColor;

	void checkSoundEffects();
	std::list<sf::Sound*> mSoundEffects;

	void checkParticles(int deltaTime);
	std::list<Particle*> mParticles;

	Gui* mGui;
	virtual void initGui();
	virtual void updateGui();
	virtual void renderGui();
};

#endif // _SCENE_INCLUDE