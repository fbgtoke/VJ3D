#include "ObjReader.h"

ObjReader::ObjReader() {}
ObjReader::~ObjReader() {}

void ObjReader::loadFromFile(const std::string& filename) {
	std::ifstream stream(filename);
	if (!stream.is_open()) {
		std::cout << "Could not open OBJ file " << filename << std::endl;
		return;
	}

  std::string line;
  while (getline(stream,line)) {
    if (line.find("v ") != std::string::npos) readVertex(line);
    else if (line.find("vn ") != std::string::npos) readNormal(line);
    else if (line.find("vt ") != std::string::npos) readTexCoord(line);
    else if (line.find("f ") != std::string::npos) readFace(line);
  }
}

void ObjReader::getVertices(float* &vertices, size_t& nelem) {
  nelem = mFaces.size() * 3 * 3;
  vertices = (float*) malloc(nelem * sizeof(float));

  Face face;
  glm::vec3 vertex;
  for (int i = 0; i < mFaces.size(); ++i) {
    face = mFaces[i];
    for (int j = 0; j < 3; ++j) {
      vertex = face.v[j];

      vertices[i*3*3 + j*3 + 0] = vertex.x;
      vertices[i*3*3 + j*3 + 1] = vertex.y;
      vertices[i*3*3 + j*3 + 2] = vertex.z;
    }
  }
}

void ObjReader::getTexCoords(float* &texcoords, size_t& nelem) {
  nelem = mFaces.size() * 3 * 2;
  texcoords = (float*) malloc(nelem * sizeof(float));

  Face face;
  glm::vec2 texcoord;
  for (int i = 0; i < mFaces.size(); ++i) {
    face = mFaces[i];
    for (int j = 0; j < 3; ++j) {
      texcoord = face.t[j];

      texcoords[i*3*2 + j*2 + 0] = texcoord.x;
      texcoords[i*3*2 + j*2 + 1] = texcoord.y;
    }
  }
}

void ObjReader::getNormals(float* &normals, size_t& nelem) {
  nelem = mFaces.size() * 3 * 3;
  normals = (float*) malloc(nelem * sizeof(float));

  Face face;
  glm::vec3 normal;
  for (int i = 0; i < mFaces.size(); ++i) {
    face = mFaces[i];
    for (int j = 0; j < 3; ++j) {
      normal = face.n[j];

      normals[i*3*3 + j*3 + 0] = normal.x;
      normals[i*3*3 + j*3 + 1] = normal.y;
      normals[i*3*3 + j*3 + 2] = normal.z;
    }
  }
}

void ObjReader::readVertex(const std::string& line) {
  std::istringstream sstream(line);

  std::string command;
  sstream >> command;

  glm::vec3 vertex;
  sstream >> vertex.x >> vertex.y >> vertex.z;

  mVertices.push_back(vertex);
}

void ObjReader::readNormal(const std::string& line) {
  std::istringstream sstream(line);

  std::string command;
  sstream >> command;

  glm::vec3 normal;
  sstream >> normal.x >> normal.y >> normal.z;

  mNormals.push_back(normal);
}

void ObjReader::readTexCoord(const std::string& line) {
  std:istringstream sstream(line);

  std::string command;
  sstream >> command;

  glm::vec2 texcoord;
  sstream >> texcoord.x >> texcoord.y;

  mTexCoords.push_back(texcoord);
}

void ObjReader::readFace(const std::string& line) {
  std::istringstream sstream(line);

  std::string command;
  sstream >> command;

  Face face;
  int v_index, t_index, n_index;
  char c;

  for (int i = 0; i < 3; ++i) {
    sstream >> v_index >> c >> t_index >> c >> n_index;
    
    // All elements are indexed by 1
    face.v[i] = mVertices[v_index-1];
    face.t[i] = mTexCoords[t_index-1];
    face.n[i] = mNormals[n_index-1];
  }

  mFaces.push_back(face);
}