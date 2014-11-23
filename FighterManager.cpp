#include "FighterManager.h"


FighterManager::FighterManager(Camera* cam)
{
	camera = cam;
	nextFighterInterval = 5000;
	lastHeight = MAX_HEIGHT;
	lastFighter = 0;
	speed = 10;	
	nextFighter(0);
}

FighterManager::~FighterManager(void)
{
	for (Fighter* fighter: fighters)
	{
		delete fighter;
	}
}

void FighterManager::recalculatePositions(int actTime, std::vector<Bullet*>* bullets, Barrel* barrel) 
{
	//recalculating:
	for (Fighter* fighter: fighters)
	{
		fighter->recalculatePosition(actTime);
		glm::vec3 pos = fighter->getPosition();
		
		// Attacking when right above the barrel
		if (pos.x < 0.5f && pos.x > -0.5f && pos.z < 0.5f && pos.z > -0.5f && !fighter->fired)
		{
			bullets->push_back(fighter->attack(actTime));
		}
	}

	// Collision detection
	for (int i = 0; i < bullets->size(); i++)
	{		
		//Checking if barrel was shot by enemy's bullet
		if (!bullets->at(i)->nice && barrel->shotBy(bullets->at(i)->getPosition()))
		{
			// explosion, maybe?
			bullets->erase(bullets->begin() + i);
			i--;
			continue;
		}

		//Checking if any fighter was shot
		for (int j = 0; j < fighters.size(); j++)
		{
			if (fighters.at(j)->shotBy(bullets->at(i)->getPosition()))
			{
				bullets->erase(bullets->begin() + i);
				i--;
				fighters.erase(fighters.begin() + j);
				// explosion, maybe?
				break;
			}
		}
	}

	// Deleting those out of range
	for (int i = 0; i < fighters.size(); i++)
	{
		glm::vec3 pos = fighters.at(i)->getPosition();
		if (pow(pos.x,2) + pow(pos.z,2) > pow(START_DISTANCE,2))
		{
			fighters.erase(fighters.begin() + i);
			i--;
		}
	}

	if (actTime < nextFighterInterval + lastFighter) return;

	// Generating new fighter:
	nextFighter(actTime);

	
	lastFighter = actTime;

	// Calculating appearance time of the next fighter
	if (nextFighterInterval > MIN_INTERVAL)
	{
		nextFighterInterval *= 0.99f;
	}
}

void FighterManager::renderFighters()
{
	for(Fighter* fighter : fighters)
	{
		if(fighter != nullptr)
		{
			fighter -> render();
		}
	}
}

std::vector<Fighter*> FighterManager::getElements()
{
	return fighters;
}

void FighterManager::nextFighter(int actTime)
{
	float x = rand() * 1.0 / RAND_MAX  * (START_DISTANCE*2) - START_DISTANCE;		
	float z = sqrt(pow(START_DISTANCE,2) - x*x);
	if (rand() % 2 == 0)
	{
		z = -z;
	}
	lastHeight -= 2;
	if (lastHeight < MIN_HEIGHT)
	{
		lastHeight = MAX_HEIGHT;
	}
	glm::vec3 pos = glm::vec3(x, lastHeight, z);
	fighters.push_back(new Fighter(camera, pos, glm::vec3(0,0,0), speed, actTime));
	if (speed < MAX_SPEED)
	{
		speed *= 1.02;
	}
}