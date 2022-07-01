#include "Fighter.h"
#include <iostream>

Fighter::Fighter(int newTeam) : numOfBullets(40), numOfGrenades(3)
{
	team = newTeam;
}

void Fighter::PickUpHealthKits()
{
	hp += 50;
	if (hp > 100)
		hp = 100;
}

void Fighter::FireBullet(int maze[MSZ][MSZ])
{
	Bullet* b = new Bullet(this->col, this->row, (rand() % 360) * 3.14 / 180);
	b->Fire();
	b->show();
	while (b->getIsMoving())
	{
		b->Move(maze);
		b->show();
	}

}