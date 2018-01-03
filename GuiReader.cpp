#include "GuiReader.h"
#include "Game.h"

Gui* GuiReader::loadFromFile(const std::string& filename) {
  std::ifstream stream("layouts/" + filename);
  if (!stream.is_open()) {
    std::cout << "Could not load layout " << filename << std::endl;
    return nullptr;
  }

  unsigned int nlayers = 0;
  readHeader(stream, nlayers);

  if (nlayers == 0) {
    std::cout << "Cannot create layout with 0 layers" << std::endl;
    return nullptr;
  }

  Gui* gui = new Gui();
  gui->init();
  gui->setNumberOfLayers(nlayers);

  unsigned int layer = 0;
  std::string line;
  while (getline(stream, line) && layer < nlayers) {
    if (!line.empty() && line == "<layer>") {
      readLayer(stream, gui, layer);
      ++layer;
    }
  }

  stream.close();

  return gui;
}

void GuiReader::readHeader(std::ifstream& stream, unsigned int& nlayers) {
  std::string line;
  getline(stream, line);

  if (line.empty()) {
    std::cout << "No header found!" << std::endl;
    return;
  }

  std::string tag = line;
  tag.erase(0, 1);
  tag.erase(line.size() - 2);

  std::stringstream sstream(tag);
  for (std::string token; getline(sstream, token, ' '); ) {
    std::string id, value;

    if (token.find("=") != -1) {
      id = token.substr(0, token.find("="));
      value = token.substr(token.find("=") + 1);
    } else {
      id = token;
      value = "";
    }

    if (id == "layers")
      nlayers = std::stoi(value);
  }
}

void GuiReader::readLayer(std::ifstream& stream, Gui* gui, unsigned int layer) {
  std::string line;

  while (getline(stream, line) && line != "</layer>") {
    if (!line.empty()) {
      std::string type;
      type = line.substr(line.find("<") + 1, line.find(">") - (line.find("<") + 1));

      Sprite* element = readElement(type, stream, gui);
      if (element != nullptr)
        gui->addSprite(element, layer);
    }
  }
}

Sprite* GuiReader::readElement(const std::string& type, std::ifstream& stream, Gui* gui) {
  if (type == "text")
    return readText(stream, gui);
  else if (type == "sprite")
    return readSprite(stream, gui);
  
  std::cout << "Unknown element " << type << std::endl;
  return nullptr;
}

Sprite* GuiReader::readText(std::ifstream& stream, Gui* gui) {
  Text* text = new Text();
  text->init();

  std::string line;
  std::string tag;
  std::string value;
  do {
    getline(stream, line);
    tag = line.substr(line.find("<") + 1, line.find(">") - (line.find("<") + 1));
    value = line.substr(line.find(">") + 1, line.find("</") - (line.find(">") + 1));
    
    std::stringstream sstream(value);

    if (tag == "position") {
      glm::vec2 position;
      sstream >> position.x >> position.y;
      text->setPosition(position);
    } else if (tag == "size") {
      float fontSize;
      sstream >> fontSize;
      text->setFontSize(fontSize);
    } else if (tag == "color") {
      glm::vec3 fontColor;
      sstream >> fontColor.x >> fontColor.y >> fontColor.z;
      text->setFontColor(fontColor);
    } else if (tag == "value") {
      text->setString(value);
    } else if (tag == "name") {
      std::string name;
      sstream >> name;
      text->setName(name);
    } else if (tag != "/text") {
      std::cout << "Unrecognized property for Text element: " << tag << std::endl;
    }

  } while (tag != "/text");

  return text;
}

Sprite* GuiReader::readSprite(std::ifstream& stream, Gui* gui) {
  Sprite* sprite = new Sprite();
  sprite->init();

  std::string line;
  std::string tag;
  std::string value;
  do {
    getline(stream, line);
    tag = line.substr(line.find("<") + 1, line.find(">") - (line.find("<") + 1));
    value = line.substr(line.find(">") + 1, line.find("</") - (line.find(">") + 1));
    
    std::stringstream sstream(value);

    if (tag == "position") {
      glm::vec2 position;
      sstream >> position.x >> position.y;
      sprite->setPosition(position);
    } else if (tag == "size") {
      glm::vec2 size;
      sstream >> size.x >> size.y;
      sprite->resize(size);
    } else if (tag == "name") {
      std::string name;
      sstream >> name;
      sprite->setName(name);
    } else if (tag == "texture") {
      std::string filename;
      sstream >> filename;

      Texture* texture;
      if (filename.find("/") == std::string::npos)
        texture = Game::instance().getResource().texture(filename);
      else
        texture = Game::instance().getResource().texture(filename, true);

      sprite->setTexture(texture);
    } else if (tag != "/sprite") {
      std::cout << "Unrecognized property for Sprite element: " << tag << std::endl;
    }

  } while (tag != "/sprite");

  return sprite;
}