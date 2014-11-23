#pragma once
#include "Renderable.h"
#include "glm\glm.hpp"
#include "Camera.h"
#include <math.h>
#include "Bullet.h"

class Fighter
{
public:
	Fighter(Camera* cam, glm::vec3 pos, glm::vec3 destination, float speed, int actTime);
	~Fighter(void);
	void recalculatePosition(int actTime);
	void render();
	glm::vec3 getPosition();
	bool shotBy(glm::vec3 bulletPos);
	bool fired;
	Bullet* attack(int actTime);

private:
	Camera* camera;
	static Renderable* mesh;
	glm::vec3 position;
	glm::vec3 direction;
	int healthPoints;
	float angle;

	float speed; // distance per sec
	int lastMove; // time of the last recalculating of the position
};

