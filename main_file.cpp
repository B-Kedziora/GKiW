#include <windows.h> //Wymagane dla implementacji OpenGL w Visual Studio.
#include <time.h>
#include <vector>
#include "gl\glew.h"
#include "gl\glut.h"
#include "stdio.h" //Przydatne do wypisywania komunikatów na konsoli
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "tga.h"
#include "cube.h"
#include "Camera.h"
#include "Skybox.h"
#include "Texture.h"
#include "Renderable.h"
#include "Light.h"
#include "Barrel.h"
//#include "Bullet.h"
#include "BulletManager.h"
#include "FighterManager.h"
#include "UFO.h"

float speed_x=0; 
float speed_y=0; 
int lastTime=0;
float angle_x;
float angle_y;

int mouseX, mouseY;

Camera* camera;

Skybox* skybox;

Renderable* beach;

Barrel* barrel;

BulletManager bullets;

Light* sun;

FighterManager* fighters;

UFO* ufo;

int actTime;

void displayFrame(void) {
	actTime=glutGet(GLUT_ELAPSED_TIME);
	glClearColor(0,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 M;
	camera -> putView();
	
	camera -> putPerspective();

	M=glm::mat4(1.0f);
	camera->putModelView(M);

	skybox->renderSkybox();

	beach->render();

	fighters->renderFighters();

	barrel->renderBarrel(actTime);	//actTime for flash

	bullets.renderBullets();

	if (ufo != nullptr)
	{
		ufo->render();
	}

	M=glm::mat4(1.0f);
	M=glm::translate(M, glm::vec3(0.0f, 0.0f, -10.0f));
	M=glm::rotate(M,angle_y,glm::vec3(0.0f,1.0f,0.0f));
	M=glm::rotate(M,angle_x,glm::vec3(1.0f,0.0f,0.0f));
	camera->putModelView(M);

	sun -> Move(glm::vec3(1.0f, 1.0f, 1.0f));

	/* CUBE
	glBindTexture(GL_TEXTURE_2D, tex->getTexture());
	
	glEnableClientState(GL_VERTEX_ARRAY);	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3,GL_FLOAT,0, cubeVertices);
	glNormalPointer(GL_FLOAT, 0, cubeNormals);
	glTexCoordPointer( 2, GL_FLOAT, 0, cubeTexCoords);

	glDrawArrays(GL_QUADS,0,cubeVertexCount);

	glDisableClientState(GL_VERTEX_ARRAY);	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY); */
	
	glutSwapBuffers();
}


void nextFrame(void) {

	actTime = glutGet(GLUT_ELAPSED_TIME);

	if (barrel->isDestroyed() || camera->getPosition().y > 20.0f) 
	{
		glutIdleFunc(NULL);
		glutHideWindow();
		printf("\n\n\nYou've survived for: %ds\n", actTime/1000);
		return;
	}
	
	camera->recalculatePosition(actTime);

	barrel->recalculatePosition(actTime);

	bullets.recalculatePositions(actTime);

	fighters->recalculatePositions(actTime, bullets.getElements(), barrel);

	if (ufo != nullptr)
	{
		if (ufo->recalculatePosition(actTime)) 
		{
			// if UFO gets to the point right above the player it starts picking him up
			//printf("UFO above\n");
			camera->setMovement(glm::vec3(0.0f, 1.0f, 0.0f), 5.0f);
		}
		ufo->animate(actTime);
		if (ufo->detectCollisions(bullets.getElements()))
		{
			ufo = nullptr;
		}
	}

	int interval=actTime-lastTime;
	//printf("Last frame generated in: %d\n", interval);
	lastTime=actTime;
	angle_x+=speed_x*interval/1000.0;
	angle_y+=speed_y*interval/1000.0;
	if (angle_x>360) angle_x-=360;
	if (angle_x>360) angle_x+=360;
	if (angle_y>360) angle_y-=360;
	if (angle_y>360) angle_y+=360;


	camera->mouseMovementHandler(mouseX, mouseY);
	
	glutPostRedisplay();
}

void keyDown(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT: 
      speed_y=60;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-60;
      break;
    case GLUT_KEY_UP: 
      speed_x=60;
      break;
    case GLUT_KEY_DOWN:
      speed_x=-60;
      break;
	case GLUT_KEY_PAGE_DOWN:
		camera->zoom(true);
		break;
	case GLUT_KEY_PAGE_UP:
		camera->zoom(false);
		break;
	case GLUT_KEY_END:
		delete skybox; //TEST
		exit(1);
		break;
  }
}

void keyUp(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT: 
      speed_y=0;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-0;
      break;
    case GLUT_KEY_UP: 
      speed_x=0;
      break;
    case GLUT_KEY_DOWN:
      speed_x=-0;
      break;  
  }
}

//Nie dzia³a. Nie widzi rolki. :(
void mouseButtons(int button, int state, int x, int y)
{
	if((button == 0) && (state == GLUT_DOWN))	//FIIIIREEEEEEEE! 
	{
		bullets.addBullet(barrel -> shoot(actTime));
	}
	else if((button == 3) || (button == 4))
	{
		if (state == GLUT_UP) return;
		(button == 3) ? camera->zoom(true) : camera->zoom(false);
	}
}

void mouseMotion(int x, int y)
{
	mouseX = x;
	mouseY = y;
}


int main(int argc, char* argv[]) {
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 640);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Program OpenGL");        
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);

	camera = new Camera(800, 640, false);

	//Tutaj kod inicjujacy	
	glewInit();
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	glutMouseFunc(mouseButtons);
	glutPassiveMotionFunc(mouseMotion); 



	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	
	skybox = new Skybox("skybox/", ".png");

	sun = new Light(glm::vec3(0.0f, 20.0f, 3.0f), true);

	barrel = new Barrel(camera);

	fighters =  new FighterManager(camera);

	ufo = new UFO(camera);

	glEnable(GL_TEXTURE_2D);

	beach = new Renderable();
	beach->loadObject("plaza/plaza.obj", "plaza");
	beach->repeatTexture(true);

    glutMainLoop();	

    return 0;
}