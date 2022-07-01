#include "Soldier.h"

Soldier::Soldier(): hp(100), alive(true), team(WHITE), row(0), col(0)
{
}

Soldier::Soldier(int team)
{

}
Soldier::~Soldier()
{

}

void Soldier::setHP(int newHp)
{
	hp = newHp;
}

void Soldier::setTeam(int team)
{
	this->team = team;
}


void Soldier::setPos(int maze[MSZ][MSZ], int newRow, int newCol)
{
	maze[row][col] = SPACE;
	row = newRow;
	col = newCol;
	maze[row][col] = team;
}

