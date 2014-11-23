#ifndef CAMERA_H
#define CAMERA_H

#include "glm\glm.hpp"
#include "gl\glew.h"
#include "gl\glut.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include <stdio.h>
#include <vector>

/*
*	Generic FPS camera class created for <NAZWA NASZEGO PROJEKTU> by Bartosz Kêdziora.
*	It uses OpenGL Mathematics (GLM) library to perform mathematical calculations and GLUT as OpenGL API. 
*	The class contains all data needed to calculate desired viewpoint, so there can be multiple cameras in project by only one active at the moment.
*	Basic possibilities offered by this class is: rotating, zooming and moving. You can also change ortho and perspective mode <SOON>.
*
*	The modes of this camera are: 
*	Normal mode: camera working simillary to normal FPS camera.
*	Artillery mode: camera can be rotated more than 180 degrees, so it can be set upside-down. It is like the camera is mounted on altazimuth mount.
*	In fact the difference is that normal mode has very usefull default constraint on vertical 90 degrees. 
*
*	Constraints:
*	You can add constraints to camera. It ensures that camera will not exceed given value. For instance you can need that your camera will never look
*	below the horizon.
*/

/*****************************PLANS FOR FUTURE DEVELOPMENT***********************************
 - Complete normal FPS camera.
 - deleteConstraint() function
 - add check to addConstraint() whether camera has already exceeded new constraint. In that case constraint shouldn't be added.
********************************************************************************************/

const float DEG_TO_RAD = 0.01745329251;
const float PI = 3.14159265359;

struct Constraint
{
	bool notLower;		//True - camera won't get lower than angle value. False - camera won't get above given value
	int angle;
	bool horizontal;	//True - horizontal. False - Vertiacal;
};

class Barrel;

class Camera
{
public:
	friend class Barrel;
	/*	Only argument are width - height ration and mode. Other values initialized with default values.		*
	*	mode: false -> normal FPS camera; true -> artillery camera mode										*/
	Camera(int width, int height, bool artilleryMode);	//TODO: POSITION
	~Camera(void);

	/*Changes the zoom by manipulating fieldOfView value. When zooming is true, then camera zoom in. When false - zoom out.*/ 
	void zoom(bool zooming);
	
	/*Calculates and applies projection (P) matrix*/
	void putPerspective();									//'PUT' MAY BE NOT THE MOST APPROPRIATE WORD

	/*Calculates view (V) matrix */
	void putView();

	/*Set V*M as modelview matrix*/
	void putModelView(glm::mat4& M);

	/*Calculates changes in direction vector*/
	void mouseMovementHandler(int mouseX, int mouseY);

	/*Self documentatory*/
	void setPosition(glm::vec3 position)
	{
		this->position = position;
	}

	/*Require values between 1 (very slow) and 1000 (very fast)*/ /*NOT TESTED YET*/
	void setMouseSpeed(int speed)
	{
		if((speed > 0) && (speed < 1000))
			mouseSpeed = (float) speed / 10000.0f;
	}

	void clearConstraints();

	/*Convention written in Camera header file*/
	void addConstraint(bool notLower, int angle, bool horizontal);

	/* Sets direction and speed of camera movement */
	void setMovement(glm::vec3 direction, float speed);

	/* Calculates the position of the camera basing on speed variable and moveDirection vector*/
	void recalculatePosition(int actTime);

	glm::vec3 getPosition();

private:
	bool artilleryMode;								//Difference between normal mode and artillery mode explained before class in file
	float fieldOfView;								//Used for zoom effect
	float widthHeightRatio;
	int width, height;
	float nearClipPlane, farClipPlane;
	float mouseSpeed;								//Camera rotation speed
	float horizontalAngle, verticalAngle;			//Used to handle mouse movements. Calculated into direction.
	glm::vec3 position, direction, upDirection;
	glm::mat4 P, V;									//Projection and view matrices
	std::vector<Constraint> constraints;			//Camera movement limiters
	glm::vec3 moveDirection;						
	float speed;									//Distance per second
	int lastMove;
};

#endif