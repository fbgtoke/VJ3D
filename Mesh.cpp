#include "Mesh.h"

Mesh::Mesh() :
  nFloatsVertices(0), nFloatsNormals(0), nFloatsTexCoords(0),
  mVertices(nullptr), mNormals(nullptr), mTexCoords(nullptr) {}

Mesh::~Mesh() {
  if (mVertices != nullptr) free(mVertices);
  if (mNormals != nullptr) free(mNormals);
  if (mTexCoords != nullptr) free(mTexCoords);
}

void Mesh::setVertices(float* vertices, size_t nelem) {
  mVertices = vertices;
  nFloatsVertices = nelem;
}

void Mesh::setNormals(float* normals, size_t nelem) {
  mNormals = normals;
  nFloatsNormals = nelem;
}

void Mesh::setTexCoords(float* texcoords, size_t nelem) {
  mTexCoords = texcoords;
  nFloatsTexCoords = nelem;
}

float* Mesh::vertices() { return mVertices; }
float* Mesh::normals() { return mNormals; }
float* Mesh::texcoords() { return mTexCoords; }

size_t Mesh::verticesSize() const { return nFloatsVertices * sizeof(float); }
size_t Mesh::normalsSize() const { return nFloatsNormals * sizeof(float); }
size_t Mesh::texcoordsSize() const { return nFloatsTexCoords * sizeof(float); }

size_t Mesh::numVertices() const { return nFloatsVertices/3; }

void Mesh::getMinMaxVertices(glm::vec3& min, glm::vec3& max) const {
  min.x = max.x = mVertices[0];
  min.y = max.y = mVertices[1];
  min.z = max.z = mVertices[2];

  for (unsigned int i = 3; i < nFloatsVertices; i+=3) {
    if (mVertices[i+0] < min.x)
      min.x = mVertices[i+0];
    if (mVertices[i+0] > max.x)
      max.x = mVertices[i+0];
    if (mVertices[i+1] < min.y)
      min.y = mVertices[i+1];
    if (mVertices[i+1] > max.y)
      max.y = mVertices[i+1];
    if (mVertices[i+2] < min.z)
      min.z = mVertices[i+2];
    if (mVertices[i+2] > max.z)
      max.z = mVertices[i+2];
  }
}

glm::vec3 Mesh::center() const {
  glm::vec3 center;

  glm::vec3 min, max;
  getMinMaxVertices(min, max);

  center.x = (min.x + max.x)/2.f;
  center.y = (min.y + max.y)/2.f;
  center.z = (min.z + max.z)/2.f;

  return center;
}

glm::vec3 Mesh::sizeInTiles() const {
  glm::vec3 s = size();

  s.x = ceil(s.x/TILE_SIZE);
  s.y = ceil(s.y/TILE_SIZE);
  s.z = ceil(s.z/TILE_SIZE);

  return s;
}

glm::vec3 Mesh::size() const {
  glm::vec3 min, max;
  getMinMaxVertices(min, max);

  glm::vec3 size = max - min;
  return size;
}