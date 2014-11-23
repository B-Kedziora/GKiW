#include "Barrel.h"

Renderable* Barrel::barrelMesh = nullptr;
float Barrel::SPECULAR[] = {0.55, 0.55, 0.55, 1.0};
float Barrel::AMBIENT[] = {0.35, 0.35, 0.35, 1.0};
float Barrel::DEF_AMBIENT[] = {0.2, 0.2, 0.2, 1.0};
float Barrel::DEF_SPECULAR[] = {0.0, 0.0, 0.0, 1.0};

Barrel::Barrel(Camera* cam)
{
	camera = cam;
	flash = nullptr;
	blood = nullptr;
	if(barrelMesh == nullptr)
	{
		barrelMesh = new Renderable();
		barrelMesh -> loadObject("lufa/lufa.obj", "lufa");
		barrelMesh ->repeatTexture(true);
	}
	lastShooting = 0;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	healthPoints = 10;
	isShot = false;
}

void Barrel::renderBarrel(int actualTime)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, SPECULAR);	//Sets specular on barrel (without this it looks like a stone)
	glMaterialfv(GL_FRONT, GL_AMBIENT, AMBIENT);
	glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
	glm::mat4 M=glm::mat4(1.0f);
	M = glm::translate(M, position);
	M = glm::rotate(M, camera->horizontalAngle/DEG_TO_RAD, glm::vec3(0.0f,1.0f,0.0f));
	M = glm::rotate(M, camera->verticalAngle/DEG_TO_RAD, glm::vec3(-1.0f,0.0f,0.0f));
	camera->putModelView(M);
	barrelMesh->render();
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, DEF_SPECULAR);	//Return to normal material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, DEF_AMBIENT);	

	// Light after shooting
	if(flash != nullptr)
	{
		lightPower -= (actualTime - lastLightCounter) / 3; //Time elapsed since last update
		lastLightCounter = actualTime;
		if(lightPower > 0)
		{
			float pow = lightPower/100.0f;
			float param[] = {pow, pow, pow, 1};
			flash->setParameter(GL_DIFFUSE, param);
		}
		else
		{
			delete flash;
			flash = nullptr;
		}
	}
	//Light after being shot
	if(blood != nullptr)
	{
		bloodPower -= (actualTime - lastBloodCounter) / 3; 
		lastBloodCounter = actualTime;
		if(bloodPower > 0)
		{
			float pow = bloodPower/100.0f;
			float param[] = {pow, pow, pow, 1};
			blood->setParameter(GL_DIFFUSE, param);
		}
		else
		{
			delete blood;
			blood = nullptr;
		}
	}
}

void Barrel::recalculatePosition(int actTime)
{
	if (actTime - lastShooting < 50)
	{
		float distance = (actTime - lastShooting) / 500.0;
		position = direction * distance;
	}
	else if (actTime - lastShooting  < COOLDOWN)
	{
		float distance = (COOLDOWN + 50 - actTime + lastShooting) / 4000.0;
		position = direction * distance;
	}
	else
	{
		direction = glm::vec3(0.0f, 0.0f, 0.0f);
		position = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// Creates bloody flash if barrel has been shot
	if (isShot)
	{
		if(blood == nullptr)
		{
			blood = new Light(glm::vec3(0.0f, 1.0f, 0.0f), false);
			float param[] = {1.0f, 0.0f, 0.0f, 1};
			blood->setParameter(GL_AMBIENT, param);
			bloodPower = 100;
			lastBloodCounter = actTime;
		}
		//explosion, maybe?
		healthPoints--;
		isShot = false;
	}
}

Bullet* Barrel::shoot(int actualTime)
{
	if(actualTime > (lastShooting + COOLDOWN))
	{
		lastShooting = actualTime;
		if(flash == nullptr)
		{
			flash = new Light(glm::vec3(0.0f, 1.0f, 0.0f), false);
			lightPower = 100;
			lastLightCounter = actualTime;
		}
		Bullet* bullet = new Bullet(actualTime, camera->verticalAngle, camera->horizontalAngle, 
									camera, glm::vec3(0.0f, 0.0f, 0.0f), true);
		direction = -glm::vec3(cos(camera->verticalAngle) * sin(camera->horizontalAngle),
							sin(camera->verticalAngle),
							cos(camera->verticalAngle) * cos(camera->horizontalAngle)
		);
		return bullet;
	}
	else
	{
		return nullptr;
	}
}

Barrel::~Barrel(void)
{
}

bool Barrel::shotBy(glm::vec3 bulletPos)
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	isShot = (bulletPos.x > position.x - 2 && bulletPos.x < position.x + 2
		&& bulletPos.y < position.y
		&& bulletPos.z > position.z - 2 && bulletPos.z < position.z + 2);
	return isShot;
}

bool Barrel::isDestroyed()
{
	return healthPoints <= 0;
}