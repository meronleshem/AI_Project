#include "Carrier.h"
#include <iostream>

Carrier::Carrier(int newTeam) :numOfAmmoSupply(0), numOfHealthKits(0)
{
	team = newTeam;
}

void Carrier::Move()
{
	std::cout << "Carrier Move" << std::endl;
}
