#ifndef _RESOURCE_MANAGER_INCLUDE
#define _RESOURCE_MANAGER_INCLUDE

#include "utils.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "ObjReader.h"
#include "Texture.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Mesh* mesh(const std::string& name);
	ShaderProgram* shader(const std::string& name);
	Texture* texture(const std::string& name);
  sf::Music* music(const std::string& name);

private:
  void loadMesh(const std::string& name);
	std::map<std::string, Mesh*> mMeshes;

  void loadShader(const std::string& name);
  std::map<std::string, ShaderProgram*> mShaders;

  void loadTexture(const std::string& name);
  std::map<std::string, Texture*> mTextures;

  void loadMusic(const std::string& name);
  std::map<std::string, sf::Music*> mMusics;
};

#endif // _RESOURCE_MANAGER_INCLUDE