#pragma once
#include "Definitions.h"
#include "Soldier.h"

class Room
{
private:
	int centerX, centerY;
	int width, height;
public:
	Room();
	~Room();

	void AddMeToMaze(int maze[MSZ][MSZ]);
	void AddSoldiersToRoom(int maze[MSZ][MSZ], Soldier* team[]);
	void setWidth(int w) { width = w; }
	void setHeight(int h) { height = h; }
	void setCenterX(int x) { centerX = x; }
	void setCenterY(int y) { centerY = y; }

	int getWidth() { return width; }
	int getHeight() { return height; }
	int getCenterX() { return centerX; }
	int getCenterY() { return centerY; }

};

