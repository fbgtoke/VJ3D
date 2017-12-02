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
#include <sstream>
#include <cmath>
#include <functional>

/* STD containers */
#include <vector>
#include <list>
#include <map>
#include <memory>

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

static const glm::vec3 UP    = glm::vec3(0.f, 1.f, 0.f);
static const glm::vec3 DOWN  = glm::vec3(0.f, -1.f, 0.f);
static const glm::vec3 RIGHT = glm::vec3(1.f, 0.f, 0.f);
static const glm::vec3 LEFT  = glm::vec3(-1.f, 0.f, 0.f);
static const glm::vec3 OUT   = glm::vec3(0.f, 0.f, 1.f);
static const glm::vec3 IN    = glm::vec3(0.f, 0.f, -1.f);

static const float TILE_SIZE  = 16;
static const float TILES_PER_CHUNK = 25;

static const float CHUNK_SIZE = TILE_SIZE * TILES_PER_CHUNK;

/* Useful macros */
#define randomFloat(min, max) \
	(min + static_cast <float> (rand())/(static_cast <float> (RAND_MAX/(max-min))))

#endif // _UTILS_INCLUDE