#ifndef _RESOURCE_MANAGER_INCLUDE
#define _RESOURCE_MANAGER_INCLUDE

#include "utils.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "ObjReader.h"
#include "Texture.h"
#include "Level.h"
#include "Gui.h"
#include "GuiReader.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Mesh* mesh(const std::string& name);
	ShaderProgram* shader(const std::string& name);
	Texture* texture(const std::string& name, bool fullPath = false);

  sf::Music* music(const std::string& name);
  sf::SoundBuffer* soundBuffer(const std::string& name);

  void loadConstants();
  int Int(const std::string& name);
  float Float(const std::string& name);

  void setInt(const std::string& name, int value);

  Gui* layout(const std::string& name);

private:
  void loadMesh(const std::string& name);
	std::map<std::string, Mesh*> mMeshes;

  void loadShader(const std::string& name);
  std::map<std::string, ShaderProgram*> mShaders;

  void loadTexture(const std::string& name, bool fullPath);
  std::map<std::string, Texture*> mTextures;

  void loadMusic(const std::string& name);
  std::map<std::string, sf::Music*> mMusics;

  void loadSoundBuffer(const std::string& name);
  std::map<std::string, sf::SoundBuffer*> mSoundBuffers;

  std::map<std::string, int> mConstantsInt;
  std::map<std::string, float> mConstantsFloat;
};

#endif // _RESOURCE_MANAGER_INCLUDE