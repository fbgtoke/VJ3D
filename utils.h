#ifndef _UTILS_INCLUDE
#define _UTILS_INCLUDE

/* GLM */
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/* GLUT */
#include <GL/glew.h>
#include <GL/glut.h>

/* STD utils */
#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>

/* STD containers */
#include <vector>
#include <list>
#include <map>

/* SFML */
#include <SFML/Audio.hpp>

/* Freetype */
#include <ft2build.h>
#include FT_FREETYPE_H

/* Ohter classes */
#include "Shader.h"
#include "ShaderProgram.h"

/* Constants */
static const unsigned int SCREEN_WIDTH  = /*854; /*/ 1280; // 1920;
static const unsigned int SCREEN_HEIGHT = /*480; /*/  720; // 1080;

#endif // _UTILS_INCLUDE