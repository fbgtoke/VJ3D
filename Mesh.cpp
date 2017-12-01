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

glm::vec3 Mesh::center() const {
  glm::vec3 center;

  float minx, miny, minz, maxx, maxy, maxz;
  minx = maxx = mVertices[0];
  miny = maxy = mVertices[1];
  minz = maxz = mVertices[2];

  for (unsigned int i = 3; i < nFloatsVertices; i+=3) {
    if (mVertices[i+0] < minx)
      minx = mVertices[i+0];
    if (mVertices[i+0] > maxx)
      maxx = mVertices[i+0];
    if (mVertices[i+1] < miny)
      miny = mVertices[i+1];
    if (mVertices[i+1] > maxy)
      maxy = mVertices[i+1];
    if (mVertices[i+2] < minz)
      minz = mVertices[i+2];
    if (mVertices[i+2] > maxz)
      maxz = mVertices[i+2];
  }

  center.x = (minx+maxx)/2.0;
  center.y = (miny+maxy)/2.0;
  center.z = (minz+maxz)/2.0;
}

float Mesh::scale() const {
  float minx, miny, maxx, maxy;
  minx = maxx = mVertices[0];
  miny = maxy = mVertices[1];

  for (unsigned int i = 3; i < nFloatsVertices; i+=3) {
    if (mVertices[i+0] < minx)
      minx = mVertices[i+0];
    if (mVertices[i+0] > maxx)
      maxx = mVertices[i+0];
    if (mVertices[i+1] < miny)
      miny = mVertices[i+1];
    if (mVertices[i+1] > maxy)
      maxy = mVertices[i+1];
  }

  return 2.0/(maxy-miny);
}