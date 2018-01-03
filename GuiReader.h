#ifndef _GUI_READER_INCLUDE
#define _GUI_READER_INCLUDE

#include "utils.h"
#include "Gui.h"
#include "Text.h"

class GuiReader {
public:
  static Gui* loadFromFile(const std::string& filename);

private:
  static void readHeader(std::ifstream& stream, unsigned int& nlayers);

  static void readLayer(std::ifstream& stream, Gui* gui, unsigned int layer);
  
  static Sprite* readElement(const std::string& type,
    std::ifstream& stream, Gui* gui);

  static Sprite* readText(std::ifstream& stream, Gui* gui);
  static Sprite* readSprite(std::ifstream& stream, Gui* gui);
};

#endif // _GUI_READER_INCLUDE