#pragma once
#include "Soldier.h"
#include "Bullet.h"
#include "Grenade.h"

class Fighter :   public Soldier
{
private:
	int numOfBullets;
	int numOfGrenades;

public:
	Fighter(int newTeam);
	int getNumOfBullets() { return numOfBullets; }
	int getNumOfGrenades() { return numOfGrenades; }
	void TakeAmmo() { numOfBullets += 20; numOfGrenades += 2; }
	void PickUpHealthKits();
	void Move(int maze[MSZ][MSZ]);
	void FireBullet(int maze[MSZ][MSZ]);
};

