#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Model.h"

class Player : public Model {
public:
	Player();
	~Player() override;

	void init() override;
	void update(int deltaTime) override;
	void render() override;
};

#endif // _PLAYER_INCLUDE