#include "Mesh.h"

Mesh::Mesh() :
  nFloatsVertices(0), nFloatsNormals(0), nFloatsTexCoords(0) {}

Mesh::~Mesh() {
  if (mVertices != nullptr) free(mVertices);
  if (mNormals != nullptr) free(mNormals);
  if (mTexCoords != nullptr) free(mTexCoords);

  freeVAO();
}

void Mesh::freeVAO() {
  glDeleteBuffers(1, &mVBO_vertices);
  glDeleteBuffers(1, &mVBO_normals);
  glDeleteBuffers(1, &mVBO_texcoord);

  glDeleteVertexArrays(1, &mVAO);
}

void Mesh::initVAO() {
  glGenVertexArrays(1, &mVAO);
  glBindVertexArray(mVAO);

  glGenBuffers(1, &mVBO_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_vertices);
  glBufferData(GL_ARRAY_BUFFER, nFloatsVertices * sizeof(float), mVertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &mVBO_normals);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_normals);
  glBufferData(GL_ARRAY_BUFFER, nFloatsNormals * sizeof(float), mNormals, GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  glGenBuffers(1, &mVBO_texcoord);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_texcoord);
  glBufferData(GL_ARRAY_BUFFER, nFloatsTexCoords * sizeof(float), mTexCoords, GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void Mesh::updateVAO() {
  glBindVertexArray(mVAO);

  glBindBuffer(GL_ARRAY_BUFFER, mVBO_vertices);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_normals);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO_texcoord);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
  
  glBindVertexArray(0);
}

GLuint Mesh::getVAO() const { return mVAO; }

void Mesh::setVertices(float* vertices, unsigned int nelem) {
  if (mVertices != nullptr) free(mVertices);

  mVertices = vertices;
  nFloatsVertices = nelem;
  updateVAO();

  getMinMaxVertices(min, max);
}

void Mesh::setNormals(float* normals, unsigned int nelem) {
  if (mNormals != nullptr) free(mNormals);

  mNormals = normals;
  nFloatsNormals = nelem;
  updateVAO();
}

void Mesh::setTexCoords(float* texcoords, unsigned int nelem) {
  if (mTexCoords != nullptr) free(mTexCoords);

  mTexCoords = texcoords;
  nFloatsTexCoords = nelem;
  updateVAO();
}

float* Mesh::vertices() { return mVertices; }
float* Mesh::normals() { return mNormals; }
float* Mesh::texcoords() { return mTexCoords; }

unsigned int Mesh::verticesSize() const { return nFloatsVertices * sizeof(float); }
unsigned int Mesh::normalsSize() const { return nFloatsNormals * sizeof(float); }
unsigned int Mesh::texcoordsSize() const { return nFloatsTexCoords * sizeof(float); }

unsigned int Mesh::numVertices() const { return nFloatsVertices/3; }

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
  glm::vec3 size = max - min;
  return size;
}