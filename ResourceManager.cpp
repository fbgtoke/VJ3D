#include "ResourceManager.h"

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {}

void ResourceManager::checkUnusedResources() {
	for (auto it = mMeshes.begin(); it != mMeshes.end();) {
		if (it->second.use_count() == 1) {
      std::cout << "Erasing mesh " + it->first << std::endl;
			mMeshes.erase(it++);
		} else {
			++it;
		}
	}

	for (auto it = mShaders.begin(); it != mShaders.end();) {
		if (it->second.use_count() == 1) {
      std::cout << "Erasing shader " + it->first << std::endl;
			mShaders.erase(it++);
		} else {
			++it;
		}
	}
}

std::shared_ptr<Mesh> ResourceManager::mesh(const std::string& name) {
	if (mMeshes.count(name) == 0)
		loadMesh(name);

	return mMeshes[name];
}

std::shared_ptr<ShaderProgram> ResourceManager::shader(const std::string& name) {
	if (mShaders.count(name) == 0)
		loadShader(name);

	return mShaders[name];
}

void ResourceManager::loadMesh(const std::string& name) {
  Mesh* mesh = new Mesh();

  ObjReader reader;
  reader.loadFromFile("models/" + name);

  float *vertices, *normals, *texcoords;
  size_t nvertices, nnormals, ntexcoords;

  reader.getVertices(vertices, nvertices);
  reader.getNormals(normals, nnormals);
  reader.getTexCoords(texcoords, ntexcoords);
  
  mesh->setVertices(vertices, nvertices);
  mesh->setNormals(normals, nnormals);
  mesh->setTexCoords(texcoords, ntexcoords);

  mMeshes[name] = std::shared_ptr<Mesh>(mesh);
}

void ResourceManager::loadShader(const std::string& name) {
	ShaderProgram* shaderProgram = new ShaderProgram();
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/" + name + ".vert");
	if(!vShader.isCompiled())
	{
		cout << "3D Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/" + name + ".frag");
	if(!fShader.isCompiled())
	{
		cout << "3D Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	shaderProgram->init();
	shaderProgram->addShader(vShader);
	shaderProgram->addShader(fShader);
	shaderProgram->link();
	if(!shaderProgram->isLinked())
	{
		cout << "3D Shader Linking Error" << endl;
		cout << "" << shaderProgram->log() << endl << endl;
	}
	shaderProgram->bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	mShaders[name] = std::shared_ptr<ShaderProgram>(shaderProgram);
}