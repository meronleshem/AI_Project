#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include<stack>
#include "Definitions.h"
#include "Cell.h"
#include "CompareCells.h"
using namespace std;

class Soldier
{
protected:
	int hp;
	int team;
	bool alive;
	int row;
	int col;
	int targertRow;
	int targetCol;
	bool hasPath;
	priority_queue<Cell*, vector<Cell*>, CmpCellF> pqAStar;
	stack<int> path;

public:
	Soldier();
	Soldier(int team);
	virtual ~Soldier();

	int getHP() { return hp; }
	int getTeam() { return team; }
	bool isAlive() { return alive; }
	int getRow() { return row; }
	int getCol() { return col; }
	bool HasPath() { return hasPath; }
	void isDead() { alive = false; }
	void setHP(int newHp);
	void setTeam(int team);
	void setPos(int maze[MSZ][MSZ], int newRow, int newCol);
	void setTarget(int tr, int tc);
	int ManhattanDistance(int row, int col, int targetRow, int targetCol);

	void CalcMove(int maze[MSZ][MSZ]);
	void Move(int maze[MSZ][MSZ]);
	void FoundPath(Cell* pCurr, int maze[MSZ][MSZ]);
	void CleanMaze(int maze[MSZ][MSZ]);

	void NewTarget(int maze[MSZ][MSZ]);
};

