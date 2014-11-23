#pragma once
#include "glm\glm.hpp"
#include "glm\gtx\rotate_vector.hpp"
#include <iostream>
#include <vector>
#include "Camera.h"
#include "Renderable.h"
#include "Bullet.h"


class UFO
{
public:
	UFO(Camera* cam);
	~UFO(void);
	bool recalculatePosition(int actTime); // returns true if already above the barrel
	void render();
	void animate(int actTime);
	bool detectCollisions(std::vector<Bullet*>* bullets); // returns true if UFO is destroyed

private:
	Camera* camera;
	static Renderable* mesh;
	glm::vec3 position;
	int lastMove;
	float angle; //for animation
	int healthPoints;
};

