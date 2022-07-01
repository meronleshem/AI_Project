#pragma once
#include "Soldier.h"
class Carrier :  public Soldier
{
private:
	int numOfAmmoSupply;
	int numOfHealthKits;

public:
	Carrier(int newTeam);
	int getAmmoSupply() { return numOfAmmoSupply; }
	int getNumOfHealthKits() { return numOfHealthKits; }
	void PickUpAmmo() { numOfAmmoSupply++; }
	void PickUpHealthKits() { numOfHealthKits++; }
	void Move();
	//void Move();
	//void Transfer();
};

