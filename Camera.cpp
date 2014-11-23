#include "Camera.h"


Camera::Camera(int width, int height, bool artilleryMode)
{
	this->width = width;
	this->height = height;
	widthHeightRatio = (float) width / (float) height;

	this->artilleryMode = artilleryMode;
	if( !artilleryMode )
		addConstraint(false, 90, false);			//Can't exceed 90 vertically
	else
		addConstraint(false, 180, false);		//Can't go below horizon upside-down.
	addConstraint(true, 0, false);				//Can't go below horizon. THESE TWO ONLY IN OUR GAME. DELETE IN FUTURE! THIS SHOULD BE PLACED SOMEWHERE IN GAME OBJECT


	fieldOfView = 50.0f;
	nearClipPlane = 1.0f;	
	farClipPlane = 500.0f;

	mouseSpeed = 0.00100f;						
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, 0.0f);
	upDirection = glm::vec3(0.0f,1.0f,0.0f);
	verticalAngle = 0.0f;
	horizontalAngle = 3.14f;

	//movement
	moveDirection = glm::vec3(0.0f, 0.0f, 0.0f);
	speed = 0;
	lastMove = 0;

	glutSetCursor(GLUT_CURSOR_NONE);				//Hide cursor.
}

Camera::~Camera(void)
{
}

void Camera::zoom(bool zooming)
{
	if(zooming)
	{
		if( fieldOfView > 20 )
		{
			fieldOfView -= 3.0f;
		}
	}
	else
	{
		if( fieldOfView < 50 )
		{
			fieldOfView += 3.0f;
		}
	}
}

void Camera::putPerspective()
{
	P = glm::perspective(fieldOfView, widthHeightRatio, nearClipPlane, farClipPlane);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));
}

void Camera::putView()
{
	V = glm::lookAt(
		position,
		direction,
		upDirection);
}

void Camera::putModelView(glm::mat4& M)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(V*M));
}

void Camera::mouseMovementHandler(int mouseX, int mouseY)
{
	glutWarpPointer(width/2, height/2);
	float horDifference = mouseSpeed * float(width/2 - mouseX );
	float verDifference = mouseSpeed * float(height/2 - mouseY );

	//CONSTRAINT CHECKING
	for(Constraint test : constraints)
	{
		if(test.horizontal)
		{
			if(test.notLower)
			{
				if( (horDifference < 0) && (horizontalAngle + horDifference < test.angle))
				{
					horDifference = 0;
				}
			}
			else
			{
				if( (horDifference > 0) && (horizontalAngle + horDifference > test.angle))
				{
					horDifference = 0;
				}
			}
		}
		else
		{
			if(test.notLower)
			{
				if( (verDifference < 0) && (verticalAngle + verDifference < test.angle))
				{
					verDifference = 0;
				}
			}
			else
			{
				if( (verDifference > 0) && (verticalAngle + verDifference > test.angle))
				{
					verDifference = 0;
				}
			}
		}
	}
	///////// END OF CONSTRAINT CHECKING

	if( verDifference || horDifference )				//To prevent unnecesary calculations
	{

		horizontalAngle += horDifference;						//Difference between current and previous mouse position
		verticalAngle   += verDifference;

		if(horizontalAngle > 2*PI)				//For our convenience angles should stay between [0; pi]  
			horizontalAngle -= 2*PI;
		if(horizontalAngle < 0)
			horizontalAngle += 2*PI;
		if(verticalAngle > 2*PI)
			verticalAngle -= 2*PI;
		if(horizontalAngle < 0)
			horizontalAngle += 2*PI;

		direction = glm::vec3(									//Spherical coordinates translated into cartesian coordinates			
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

		glm::vec3 right = glm::vec3(							//Needed to calculate up direction
		sin(horizontalAngle - PI/2.0f),
		0,
		cos(horizontalAngle - PI/2.0f)
		);

		upDirection = glm::cross( right, direction);
	}
}

void Camera::clearConstraints()
{
	constraints.clear();
	if(!artilleryMode)
		addConstraint(false, 90, false);
}

void Camera::addConstraint(bool notLower, int angle, bool horizontal)
{
	if((angle >= 0) && (angle < 360))
	{
		Constraint con = {notLower, angle * DEG_TO_RAD, horizontal};
		constraints.push_back(con);												
	}
}

void Camera::setMovement(glm::vec3 direction, float speed)
{
	this->moveDirection = direction;
	this->speed = speed;
}

void Camera::recalculatePosition(int actTime)
{
	float distance = speed * (actTime - lastMove) / 1000.0;
	position += moveDirection * distance;
	lastMove = actTime;
}

glm::vec3 Camera::getPosition()
{
	return position;
}