#pragma once
#include "Definitions.h"
const double SPEED = 1.5;

class Bullet
{

private:
	double x, y;
	double direction_angle;
	bool isMoving;
public:
	Bullet();
	Bullet(double x, double y, double angle);
	void show();
	~Bullet();
	void Fire() { isMoving = true; }
	void Move(int maze[MSZ][MSZ]);
	bool getIsMoving() { return isMoving; }
	void SimulateMotion(int maze[MSZ][MSZ], double security_map[MSZ][MSZ], double damage);
	void SimulateVisibility(int maze[MSZ][MSZ], double visibility_map[MSZ][MSZ]);

};

