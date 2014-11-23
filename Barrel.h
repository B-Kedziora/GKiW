#pragma once
#include "Camera.h"
#include "Renderable.h"
#include "glm\glm.hpp"	
#include "Light.h"
#include "Bullet.h"
#include "glm\gtx\rotate_vector.hpp"

const int COOLDOWN = 500; //minimum 500 ms between shooting

/*
* Class displaying barrel on the screen. It moves along the camera.
* It's also responsible for shooting (cooldown and light)
*/
class Barrel
{
public:
	/*Camera which angle will be used to rotate*/
	Barrel(Camera* cam);

	/*Renders chicken. ActualTime needed to manage flash*/
	void renderBarrel(int actualTime);

	/*Barrel moves a bit after shooting*/
	void recalculatePosition(int actualTime);

	/*Create Bullet, creates flash, create a lot of mess*/
	Bullet* shoot(int actualTime);

	bool shotBy(glm::vec3 bulletPos);

	bool isDestroyed();

	~Barrel(void);
private:
	Camera* camera;					//Stores camera pointer for quick use
	static Renderable* barrelMesh;	//One mesh for all barrels at screen (probably there will always be only one)
	Light* flash;					//Flash after shooting (Is there a name for it?)
	int lastLightCounter;			//Use to dim the light
	int lightPower;					//Flash power

	Light* blood;					//Red light to signal being shot
	int lastBloodCounter;
	int bloodPower;

	int lastShooting;				//For cooldown and reloading of barrel
	bool isShot;					//For BLOOD 
	int healthPoints;

	glm::vec3 position;
	glm::vec3 direction;

	static float SPECULAR[];		//Parameters to make material a bit more metalic
	static float AMBIENT[];
	static float DEF_AMBIENT[];
	static float DEF_SPECULAR[];
};

