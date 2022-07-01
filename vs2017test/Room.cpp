#include "Room.h"
#include <iostream>


Room::Room()
{
}


Room::~Room()
{
}

void Room::AddMeToMaze(int maze[MSZ][MSZ])
{
	int i, j;

	int top, bottom, left, right;
	top = centerY + height/2;
	bottom = centerY - height / 2;
	left = centerX - width / 2;
	right = centerX + width / 2;

	for (i = bottom; i <= top ; i++)
		for (j = left; j <= right; j++)
			maze[i][j] = SPACE;


	int x, y;
	for (int i = 0; i < 2; i++)
	{
		x = rand() % (top - bottom + 1) + bottom;
		y = rand() % (right - left + 1) + left;	
		maze[x][y] = AMMO;
	}

	for (int i = 0; i < 2; i++)
	{
		x = rand() % (top - bottom + 1) + bottom;
		y = rand() % (right - left + 1) + left;
		maze[x][y] = HP;
	}

}

void Room::AddSoldiersToRoom(int maze[MSZ][MSZ], Soldier* team[])
{
	int top, bottom, left, right, x, y;
	int teamColor = WHITE;
	if (team[0]->getTeam() == BLACK)
		teamColor = BLACK;
	top = centerY + height / 2;
	bottom = centerY - height / 2;
	left = centerX - width / 2;
	right = centerX + width / 2;
	for (int i = 0; i < 3; i++)
	{
		x = rand() % (top - bottom + 1) + bottom;
		y = rand() % (right - left + 1) + left;
		team[i]->setPos(maze, x, y);
	}
}
