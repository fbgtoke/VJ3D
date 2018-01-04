#ifndef _MESH_INCLUDE
#define _MESH_INCLUDE

#include "utils.h"
#include "ShaderProgram.h"

class Mesh {
public:
	Mesh();
	~Mesh();

  void useShader(ShaderProgram* shaderProgram);

  //void initVAO();
  GLuint getVAO() const;

  void setVertices(float* vertices, unsigned int nelem);
  void setNormals(float* normals, unsigned int nelem);
  void setTexCoords(float* texcoords, unsigned int nelem);

  float* vertices();
  float* normals();
  float* texcoords();

  unsigned int verticesSize() const;
  unsigned int normalsSize() const;
  unsigned int texcoordsSize() const;

  unsigned int numVertices() const;

  void getMinMaxVertices(glm::vec3& min, glm::vec3& max) const;

  glm::vec3 center() const;
  glm::vec3 sizeInTiles() const;
  glm::vec3 size() const;

protected:
  GLuint mVAO;
  GLuint mVBO_vertices, mVBO_normals, mVBO_texcoord;
  GLuint mLoc_vertices, mLoc_normals, mLoc_texcoord;

  unsigned int nFloatsVertices;
  unsigned int nFloatsNormals;
  unsigned int nFloatsTexCoords;

  glm::vec3 min, max;

  ShaderProgram* mShader = nullptr;

  float* mVertices = nullptr;
  float* mNormals = nullptr;
  float* mTexCoords = nullptr;
};

#endif // _MESH_INCLUDE