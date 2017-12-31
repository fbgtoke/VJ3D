#ifndef _TEXT3D_INCLUDE
#define _TEXT3D_INCLUDE

#include "utils.h"
#include "Mesh.h"
#include "Model.h"

class Text3D : public Model {
public:
  Text3D();
  ~Text3D();

  void render() override;

  void setString(const std::string& str);
  void setPosition(const glm::vec3& position);

private:
  static bool mInitialized;
  static std::map<char, Mesh*> mCharacters;
  static void initCharacters();

  std::string mString;
};

#endif // _TEXT3D_INCLUDE