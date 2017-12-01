#ifndef _RESOURCE_MANAGER_INCLUDE
#define _RESOURCE_MANAGER_INCLUDE

#include "Mesh.h"
#include "ShaderProgram.h"
#include "ObjReader.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	void checkUnusedResources();

	std::shared_ptr<Mesh> mesh(const std::string& name);
	std::shared_ptr<ShaderProgram> shader(const std::string& name);

private:
  void loadMesh(const std::string& name);
	std::map<std::string, std::shared_ptr<Mesh>> mMeshes;

  void loadShader(const std::string& name);
  std::map<std::string, std::shared_ptr<ShaderProgram>> mShaders;
};

#endif // _RESOURCE_MANAGER_INCLUDE