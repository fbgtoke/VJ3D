#ifndef _MESH_INCLUDE
#define _MESH_INCLUDE

#include "utils.h"

class Mesh {
public:
	Mesh();
	~Mesh();

  void setVertices(float* vertices, size_t nelem);
  void setNormals(float* normals, size_t nelem);
  void setTexCoords(float* texcoords, size_t nelem);

  float* vertices();
  float* normals();
  float* texcoords();

  size_t verticesSize() const;
  size_t normalsSize() const;
  size_t texcoordsSize() const;

  size_t numVertices() const;

  void getMinMaxVertices(glm::vec3& min, glm::vec3& max) const;

  glm::vec3 center() const;
  glm::vec3 sizeInTiles() const;
  glm::vec3 size() const;

protected:
  float* mVertices;
  float* mNormals;
  float* mTexCoords;

  size_t nFloatsVertices;
  size_t nFloatsNormals;
  size_t nFloatsTexCoords;
};

#endif // _MESH_INCLUDE