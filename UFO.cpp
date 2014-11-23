#include "UFO.h"

Renderable* UFO::mesh = nullptr;

const float ROTATE_SPEED = 160; //degrees per second
const float MOVEMENT_SPEED = 15; //distance per second
const float MIN_HEIGHT = 20;

UFO::UFO(Camera* cam)
{
	if(mesh == nullptr)
	{
		mesh = new Renderable();
		mesh->loadObject("UFO/UFO.obj", "UFO");
	}
	position = glm::vec3(150.0f, 50.0f, 150.0f); 
	camera = cam;
	lastMove = glutGet(GLUT_ELAPSED_TIME);
	angle = 0;
	healthPoints = 4;
}

UFO::~UFO(void)
{
}

void UFO::render()
{
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::translate(M, position);
	M = glm::scale(M, glm::vec3(0.5f, 0.5f, 0.5f));
	M = glm::rotate(M, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, 7.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	camera->putModelView(M);
	mesh->render();
}

bool UFO::recalculatePosition(int actTime)
{
	glm::vec3 direction = glm::vec3(-position.x, MIN_HEIGHT - position.y, -position.z); //Movement toward the point above the barrel
	glm::vec3 satellite = glm::rotate(direction, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)); // Movement around the barrel

	float distance = MOVEMENT_SPEED * (actTime - lastMove) / 1000.0;
	position += glm::normalize(glm::normalize(satellite) * 8.0f + glm::normalize(direction)) * distance;

	if (position.y < MIN_HEIGHT + 0.1 && position.z < 1 && position.x < 1 
		&& position.z > -1 && position.x > - 1) // return true if above the barrel
	{
		return true;
	}

	return false;
}

void UFO::animate(int actTime)
{
	angle += (actTime - lastMove)/1000.0 * ROTATE_SPEED;
	lastMove = actTime;
	if (angle > 360)
	{
		angle -= 360;
	}
}

bool UFO::detectCollisions(std::vector<Bullet*>* bullets)
{
	for (int i =0; i < bullets->size(); i++)
	{
		glm::vec3 pos = bullets->at(i)->getPosition();
		if (pos.x < position.x + 3 && pos.x > position.x - 3
			&& pos.y < position.y + 3 && pos.y > position.y - 3
			&& pos.z < position.z + 3 && pos.z > position.z - 3)
		{
			bullets->erase(bullets->begin() + i);
			healthPoints--;
			if (healthPoints == 0)
			{
				// explosion, maybe?
				return true;
			}
			return false;
		}
	}
	return false;
}