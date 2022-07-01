#pragma once
#include "Definitions.h"

class Soldier
{
protected:
	int hp;
	int team;
	bool alive;
	int row;
	int col;

public:
	Soldier();
	Soldier(int team);
	virtual ~Soldier();

	int getHP() { return hp; }
	int getTeam() { return team; }
	bool isAlive() { return alive; }
	int getRow() { return row; }
	int getCol() { return col; }
	void isDead() { alive = false; }
	void setHP(int newHp);
	void setTeam(int team);
	void setPos(int maze[MSZ][MSZ], int newRow, int newCol);
	virtual void Move() {};
	

};

