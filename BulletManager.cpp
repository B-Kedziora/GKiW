#include "BulletManager.h"


BulletManager::BulletManager(void)
{
}

void BulletManager::recalculatePositions(int actTime)
{
	for(Bullet* bullet : bullets)
	{
		if(bullet != nullptr)
			bullet -> recalculatePosition(actTime);
	}

	/*Deleting out of range bullets*/
	int actualSize = bullets.size();
	for(int i = 0; i < actualSize; i++)
	{
		if((bullets.at(i) == nullptr)  || (bullets.at(i)-> outOfRange))
		{
			bullets.erase(bullets.begin() + i--);
			actualSize--;
		}
	}
}

void BulletManager::renderBullets()
{
	for(Bullet* bullet : bullets)
	{
		if(bullet != nullptr)
			bullet -> render();
	}
}

void BulletManager::addBullet(Bullet* bullet)
{
	bullets.push_back(bullet);
}

BulletManager::~BulletManager(void)
{
	for(Bullet* bullet : bullets)
	{
		delete bullet;
	}
}

std::vector<Bullet*>* BulletManager::getElements()
{
	return &bullets;
}
