#include "Bullet.h"

Renderable* Bullet::mesh = nullptr;

Bullet::Bullet(int time, float vertical, float horizontal, Camera* cam, glm::vec3 pos, bool nice)
{
	if(mesh == nullptr)
	{
		mesh = new Renderable();
		mesh -> loadObject("pocisk/bullet.obj", "pocisk");
	}
	position = pos;
	firedPos = pos;
	Bullet::nice = nice;
	firedTime = time;
	verticalAngle = vertical/DEG_TO_RAD;
	horizontalAngle = horizontal/DEG_TO_RAD;
	camera = cam;
	outOfRange = false;

	/*Calculation of direction vector*/
	direction = 
	glm::vec3(									//Exactly the same we used in camera	
		cos(vertical) * sin(horizontal),
		sin(vertical),
		cos(vertical) * cos(horizontal)
		);
}

void Bullet::render()
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, position);
	M = glm::rotate(M, horizontalAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, verticalAngle, glm::vec3(-1.0f, 0.0f, 0.0f));
	camera->putModelView(M);
	mesh -> render();
}

void Bullet::recalculatePosition(int actualTime)
{
	if( actualTime > (firedTime + BULLET_LIFETIME))	//Mark bullet as ready to delete
	{
		outOfRange = true;
	}

	float distance = (actualTime - firedTime)/BULLET_SPEED_DIV;	

	position = firedPos + direction * distance;				//Moves along direction vector
	position.y -= 0.62f; //Correction for barrel
}


Bullet::~Bullet(void)
{
}

glm::vec3 Bullet::getPosition()
{
	return position;
}
