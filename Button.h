#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE

#include "Sprite.h"
#include "utils.h"

class Button : public Sprite {
public:
	Button(
		const glm::vec2 &quadSize,
		Texture* spritesheet,
		ShaderProgram* program,
		const glm::vec2& position,
		std::function<void()> onClick);

	void update(int deltaTime);

	void setOnClick(std::function<void()> onClick);
	void click();

	enum ButtonState {
		IDLE,
		HOVER,
		ACTIVE
	};

private:
	ButtonState mState;
	std::function<void()> mOnClick;

	void changeState(ButtonState state);

	bool inBounds(const glm::ivec2& position);
};

#endif // _BUTTON_INCLUDE