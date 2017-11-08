#ifndef _CURSOR_INCLUDE
#define _CURSOR_INCLUDE

#include "Cube.h"
#include "Model.h"

class Cursor : public Cube {
public:
	Cursor(const Model& model, ShaderProgram& program);
	~Cursor();

	void init() override;
	void render();

	void move(const glm::vec3& movement);

private:
	const Model& mModel;

	glm::vec3 mCurrentIndex;
};

#endif // _CURSOR_INCLUDE