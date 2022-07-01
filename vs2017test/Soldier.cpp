#include "Soldier.h"
#include <time.h>
#include <iostream>
using namespace std;
Soldier::Soldier(): hp(100), alive(true), team(WHITE), row(0), col(0), hasPath(false)
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

void Soldier::setTarget(int tr, int tc)
{
	targertRow = tr;
	targetCol = tc;
	pqAStar = priority_queue<Cell*, vector<Cell*>, CmpCellF>();
	pqAStar.push(new Cell(row, col, nullptr));
}

void Soldier::CalcMove(int maze[MSZ][MSZ])
{
	if (pqAStar.empty()|| pqAStar.size() > 200) // nothing to do
	{
		NewTarget(maze);
		return;
	}

	maze[this->row][this->col] = team;
	int row, col;

	Cell* pCurrent = pqAStar.top();
	pqAStar.pop();

	row = pCurrent->getRow();
	col = pCurrent->getCol();
	maze[row][col] = GRAY;
	int nextRow, nextCol, tmpH, tmpG, tmpF;
	tmpG = pCurrent->getG() + 1;

	for (int i = 0; i < NUM_OF_NEIGHBORS; i++)
	{
		nextRow = row + ROW_DIRECTIONS[i];
		nextCol = col + COL_DIRECTIONS[i];

		int nextCell = maze[nextRow][nextCol];
		if (nextCell != SPACE)
			continue;

		if (nextRow == targertRow && nextCol == targetCol)
		{
			FoundPath(pCurrent, maze);
			break;
		}

		tmpH = ManhattanDistance(nextRow, nextCol, targertRow, targetCol);
		tmpF = tmpH + tmpG;
		Cell* neighbor = new Cell(nextRow, nextCol, pCurrent, tmpG, targertRow, targetCol);
		pqAStar.push(neighbor);
	}
}

void Soldier::Move(int maze[MSZ][MSZ])
{
	if (path.empty())
	{
		hasPath = false;
		NewTarget(maze);
		return;
	}
	maze[this->row][this->col] = SPACE;

	this->col = path.top();
	path.pop();
	this->row = path.top();
	path.pop();

	maze[this->row][this->col] = team;
}

int Soldier::ManhattanDistance(int row, int col, int targetRow, int targetCol)
{
	return abs(row - targetRow) + abs(col - targetCol);
}

void Soldier::FoundPath(Cell* pCurr, int maze[MSZ][MSZ])
{
	while (pCurr->getParent() != nullptr)
	{
		path.push(pCurr->getRow());
		path.push(pCurr->getCol());
		pCurr = pCurr->getParent();
	}

	CleanMaze(maze);
	hasPath = true;
}

void Soldier::CleanMaze(int maze[MSZ][MSZ])
{
	for (int i = 0; i < MSZ; i++)
	{
		for (int j = 0; j < MSZ; j++)
		{
			if (maze[i][j] == GRAY)
			{
				maze[i][j] = SPACE;
			}
		}
	}
}

void Soldier::NewTarget(int maze[MSZ][MSZ])
{
	targertRow = rand() % MSZ;
	targetCol = rand() % MSZ;
	while (maze[targertRow][targetCol] != SPACE)
	{
		targertRow = rand() % MSZ;
		targetCol = rand() % MSZ;
	}
	CleanMaze(maze);
	pqAStar = priority_queue<Cell*, vector<Cell*>, CmpCellF>();
	pqAStar.push(new Cell(row, col, nullptr));

}