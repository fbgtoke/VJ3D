#ifndef _TEXT3D_INCLUDE
#define _TEXT3D_INCLUDE

#include "utils.h"
#include "Mesh.h"
#include "Model.h"

class Text3D {
public:
  Text3D();
  ~Text3D();

  void setString(const std::string& str);
  void setPosition(const glm::vec3& position);

  void setVisible(bool visible);

  float getCharacterWidth() const;

private:
  static bool mInitialized;
  static std::map<char, Mesh*> mCharacters;
  static void initCharacters();

  std::string mString;
  std::vector<Model*> mModels;

  glm::vec3 mPosition;
};

#endif // _TEXT3D_INCLUDE