#pragma once
#include <vector>
#include "glm\glm.hpp"
#include "Fighter.h"
#include "Camera.h"
#include <time.h>
#include <iostream>
#include "Bullet.h"
#include "Barrel.h"

const float START_DISTANCE = 200;
const float MAX_HEIGHT = 12;
const float MIN_HEIGHT = 6;
const float MAX_SPEED = 20;
const float MIN_INTERVAL = 1500;

class FighterManager
{
public:
	FighterManager(Camera* cam);
	~FighterManager(void);
	void recalculatePositions(int actTime, std::vector<Bullet*>* bullets, Barrel* barrel);
	void renderFighters();
	std::vector<Fighter*> getElements();
	void nextFighter(int actTime);

private:
	std::vector<Fighter*> fighters;
	Camera* camera;
	float lastHeight;
	float nextFighterInterval;
	int lastFighter;
	float speed;
};

