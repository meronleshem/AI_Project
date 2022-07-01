#include "Bullet.h"
#include "glut.h"
#include <math.h>


Bullet::Bullet()
{
}

// angle must be in radians
Bullet::Bullet(double x, double y, double angle)
{
	this->x = x;
	this->y = y;
	direction_angle = angle;
	isMoving = false;
}

void Bullet::show()
{
	glColor3d(0, 0, 0);
	glBegin(GL_POLYGON);
		glVertex2d(x - 0.5, y);
		glVertex2d(x , y + 0.5);
		glVertex2d(x + 0.5, y);
		glVertex2d(x , y - 0.5);
	glEnd();
}


Bullet::~Bullet()
{
}

void Bullet::Move(int maze[MSZ][MSZ])
{
	double dx, dy;
	if (isMoving)
	{
		dx = cos(direction_angle);
		dy = sin(direction_angle);
		x += dx * SPEED;
		y += dy * SPEED;
		if (maze[(int)y][(int)x] == WALL)
			isMoving = false;
	}

}

void Bullet::SimulateMotion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double damage)
{
	double dx, dy;
	dx = cos(direction_angle);
	dy = sin(direction_angle);
	while (isMoving)
	{
		x += dx * SPEED;
		y += dy * SPEED;

		security_map[(int)y][(int)x] += damage; // drawing in map

		if (maze[(int)y][(int)x] == WALL)
			isMoving = false;
	}

}

void Bullet::SimulateVisibility(int maze[MSZ][MSZ], double visibility_map[MSZ][MSZ])
{
	double dx, dy;
	dx = cos(direction_angle);
	dy = sin(direction_angle);
	while (isMoving)
	{
		x += dx * SPEED;
		y += dy * SPEED;

		visibility_map[(int)y][(int)x] = 1; // drawing in map

		if (maze[(int)y][(int)x] == WALL)
			isMoving = false;
	}

}
