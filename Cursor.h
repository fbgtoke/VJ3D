#ifndef _CURSOR_INCLUDE
#define _CURSOR_INCLUDE

#include "Model.h"

class Cursor : public Model {
public:
	Cursor(const Model& model, ShaderProgram& program);
	~Cursor();

	void init() override;
	void update(int deltaTime) override;
	void render() override;

	glm::mat4 getTransform() const override;

	void move(const glm::vec3& movement) override;

	glm::vec3 getCurrentIndex() const;

private:
	const Model& mParentModel;

	glm::vec3 mCurrentIndex;
};

#endif // _CURSOR_INCLUDE