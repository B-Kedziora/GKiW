#pragma once
#include "Renderable.h"
#include "glm\glm.hpp"
#include "Camera.h"
#include <iostream>

const int BULLET_LIFETIME = 5000;
const int BULLET_SPEED_DIV = 35;

class BulletManager;

class Bullet
{
public:
	friend class BulletManager;
	Bullet(int time, float vertical, float horizontal, Camera* cam, glm::vec3 pos, bool nice);
	~Bullet(void);
	void recalculatePosition(int actualTime);
	void render();
	glm::vec3 getPosition();
	bool nice; //Nice only for you, not for your enemies.

private:
	Camera* camera;
	glm::vec3 position;
	glm::vec3 firedPos;
	static Renderable* mesh;
	float verticalAngle, horizontalAngle; //Magical bullets unaffected by gravity! 
	glm::vec3 direction; //Derived from two above. We are keeping them to make rotation easier.
	int firedTime;
	bool outOfRange;
};

