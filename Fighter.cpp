#include "Fighter.h"

Renderable* Fighter::mesh = nullptr;

Fighter::Fighter(Camera* cam, glm::vec3 pos, glm::vec3 destination, float speed, int actTime)
{
	if(mesh == nullptr)
	{
		mesh = new Renderable();
		mesh->loadObject("samolot/samolot.obj", "samolot");
	}
	camera = cam;
	position = pos;
	angle = atan((pos.x - destination.x)/(pos.z - destination.z)) / DEG_TO_RAD;
	if (pos.z < destination.z) 
	{
		angle += 180;
	}
	direction = glm::normalize(glm::vec3(destination.x - pos.x, 0, destination.z - pos.z));
	lastMove = actTime;
	Fighter::speed = speed;
	fired = false;
}


Fighter::~Fighter(void)
{
}

void Fighter::recalculatePosition(int actTime)
{
	float distance = speed * (actTime - lastMove) / 1000.0f;
	position += distance * direction;
	lastMove = actTime;
}

void Fighter::render()
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, position);
	M = glm::scale(M, glm::vec3(0.5f,0.5f,0.5f));
	M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	camera->putModelView(M);
	mesh->render();
}

glm::vec3 Fighter::getPosition()
{
	return position;
}

bool Fighter::shotBy(glm::vec3 bulletPos)
{
	return (bulletPos.x > position.x - 3 && bulletPos.x < position.x + 3
		&& bulletPos.y > position.y - 2 && bulletPos.y < position.y + 2
		&& bulletPos.z > position.z - 3 && bulletPos.z < position.z + 3);
}

Bullet* Fighter::attack(int actTime)
{
	fired = true;
	return new Bullet(actTime, -90.0f*DEG_TO_RAD, 0, camera, position - glm::vec3(0.0f, 4.0f, 0.0f), false);
}