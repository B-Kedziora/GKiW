#pragma once
#include <vector>
#include "Bullet.h"

/*
*	Let's keep those bullets in line.
*	Make deleting objects easier
*/
class BulletManager
{
public:
	BulletManager(void);
	~BulletManager(void);

	void recalculatePositions(int actTime);
	void renderBullets();
	void addBullet(Bullet* bullet);
	std::vector<Bullet*>* getElements();
private:
	std::vector<Bullet*> bullets;
};

