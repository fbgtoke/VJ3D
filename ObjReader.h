#ifndef _OBJ_READER_INCLUDE
#define _OBJ_READER_INCLUDE

#include "utils.h"

class ObjReader {
public:
	ObjReader();
	~ObjReader();

	void loadFromFile(const std::string& filename);

  void getVertices(float* &vertices, size_t& nelem);
  void getTexCoords(float* &texcoords, size_t& nelem);
  void getNormals(float* &normals, size_t& nelem);

private:
	std::string filename;

	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec2> mTexCoords;
	std::vector<glm::vec3> mNormals;

	struct Face {
		glm::vec3 v[3];
    glm::vec2 t[3];
    glm::vec3 n[3];
	};
	std::vector<Face> mFaces;

  void readVertex(const std::string& line);
  void readNormal(const std::string& line);
  void readTexCoord(const std::string& line);
  void readFace(const std::string& line);
};

#endif // _OBJ_READER_INCLUDE