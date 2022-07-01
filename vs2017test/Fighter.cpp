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
void Fighter::Move()
{
	std::cout << "Fighter Move" << std::endl;
}
