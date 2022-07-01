#include "glut.h"
#include <time.h>
#include <vector>
#include <queue>
#include <iostream>
#include "Room.h"
#include "Cell.h"
#include "CompareCells.h"
#include "Bullet.h"
#include "Grenade.h"
#include "Soldier.h"
#include "Carrier.h"
#include "Fighter.h"

using namespace std;

const int NUM_ROOMS = 3;
const int W = 600;
const int H = 600;
const int NUM_OF_SOLDIERS = 3;


int maze[MSZ][MSZ] = { 0 };
double security_map[MSZ][MSZ] = { 0 };
double visibility_map[MSZ][MSZ] = { 0 };
Soldier* whiteTeam[NUM_OF_SOLDIERS];
Soldier* blackTeam[NUM_OF_SOLDIERS];

Room rooms[NUM_ROOMS];
bool underConstruction = true;
int r1=0, r2=1; // rooms indices

Bullet* pWhiteBullets[2];
Bullet* pBlackBullets[2];
Grenade* pg = nullptr;


void SetupRooms();
void DigPassages();
void AddObstacles();
void AddSoldiers();

void init()
{	//          Red Green Blue 
	glClearColor(0, 0, 0.4, 0);// color of window background
	
	// set the main axes
	glOrtho(0, MSZ, 0, MSZ ,- 1, 1); 

	srand(time(0));

	SetupRooms();
//	DigPassages();
	AddObstacles();

	AddSoldiers();

}

void AddSoldiers()
{
	int tr = rand() % MSZ;
	int tc = rand() % MSZ;
	while (maze[tr][tc] != SPACE)
	{
		tr = rand() % MSZ;
		tc = rand() % MSZ;
	}


	for (int i = 0; i < NUM_OF_SOLDIERS - 1; i++)
	{
		whiteTeam[i] = new Fighter(WHITE);
		blackTeam[i] = new Fighter(BLACK);
	}

	whiteTeam[NUM_OF_SOLDIERS - 1] = new Carrier(WHITE);
	blackTeam[NUM_OF_SOLDIERS - 1] = new Carrier(BLACK);

	rooms[0].AddSoldiersToRoom(maze, whiteTeam);
	rooms[NUM_ROOMS - 1].AddSoldiersToRoom(maze, blackTeam);

	for (int i = 0; i < NUM_OF_SOLDIERS; i++)
	{
		whiteTeam[i]->setTarget(tr, tc);
		blackTeam[i]->setTarget(tr, tc);
	}
}
void AddObstacles()
{
	int num_obstacles = 100;
	for (int i = 0; i < num_obstacles; i++)
		maze[rand() % MSZ][rand() % MSZ] = WALL;
	
}

// checks for overlapping with rooms of lower indices
bool HasOverlap(int w, int h, int x, int y, int index)
{
	bool overlaps = false;
	int i;
	int dx, dy,gap=4;

	for (i = 0; i < index && !overlaps; i++)
	{
		dx = abs(rooms[i].getCenterX() - x);
		dy = abs(rooms[i].getCenterY() - y);
		if (dx < w / 2 + rooms[i].getWidth() / 2 + gap && dy < h / 2 + rooms[i].getHeight() / 2 + gap)
			overlaps = true;
	}
	return overlaps;
}

void SetupRooms()
{
	int w, h, x, y;
	for (int r = 0; r < NUM_ROOMS; r++)
	{
		do
		{
			w = 8 + rand() % 25;
			h = 8 + rand() % 25;
			x = 2 + w / 2 + rand() % (MSZ - 4 - w);
			y = 2 + h / 2 + rand() % (MSZ - 4 - h);
		} while (HasOverlap(w,h,x,y,r));
		rooms[r].setWidth(w);
		rooms[r].setHeight(h);
		rooms[r].setCenterX(x);
		rooms[r].setCenterY(y);

		rooms[r].AddMeToMaze(maze);
	}
}

// if the neighbor cell is white  (including target as wll) then add it to pq and paint it gray
// if it is black then do nothing
// if it is gray (it can be target as wll) check 2 cases:
//         1: if F of the new found neghbor isn't better (>=) then of the previously found cell then do notheing
//         2: if  F of the new found neghbor IS better (<) then we have to update the cell parameters!!!

void CheckNeighbor(int row, int col, Cell* pcurrent, priority_queue <Cell, vector<Cell>, CompareCells>&pq,
	vector <Cell> &grays, vector <Cell>&blacks)
{
	double cost,cheap = 0.1, expensive = 0.4;
	vector <Cell>::iterator itrb;
	vector <Cell>::iterator itrg;


	if (maze[row][col] == SPACE) cost = cheap;
	else cost = expensive;

	// create neighbor cell
	Cell* pn = new Cell(row, col, pcurrent, pcurrent->getG() + cost, pcurrent->getTargetRow(), pcurrent->getTargetCol());
	// check color of theis cell
	itrb = find(blacks.begin(), blacks.end(), *pn);
	itrg = find(grays.begin(), grays.end(), *pn);

	// if it is white
	if (itrb == blacks.end() && itrg == grays.end())
	{
		pq.push(*pn);
		grays.push_back(*pn);
	}
	else  // it must be either gray or black 
		if (itrg != grays.end())
	{
		if (pn->getF() < itrg->getF()) // we have to update it
		{
			grays.erase(itrg);
			grays.push_back(*pn);
			// to update (find) pn in pq we need manually to extract all the cells that stand bfore pn in pq including pn
			// to update it and then to push all of them back to pq
			vector <Cell> tmp;
			while(!pq.empty() && !((*pn)== pq.top()))
			{
				tmp.push_back(pq.top());
				pq.pop();
			}
			if (pq.empty())
				cout << " neighbor wasn't found in pq \n";
			else // *pn is now at top of pq
			{
				pq.pop();
				pq.push(*pn);
				// move back to pq all the cells from tmp
				while (!tmp.empty())
				{
					pq.push(tmp.back());
					tmp.pop_back();
				}
			}
		}
	}

}

void RestorePath(Cell* ps)
{
	while (ps->getParent() != nullptr)
	{
		if (maze[ps->getRow()][ps->getCol()] == WALL)
			maze[ps->getRow()][ps->getCol()] = SPACE;
		ps = ps->getParent();
	}
}

// creates path from rooms[index1] to rooms[index2] using A*
void DigPath(int index1, int index2)
{
	Cell* startingCell = new Cell(rooms[index1].getCenterY(), rooms[index1].getCenterX(),
		nullptr, 0, rooms[index2].getCenterY(), rooms[index2].getCenterX());

	Cell* pcurrent = nullptr;
	int currentRow, currentCol;

	priority_queue <Cell, vector<Cell>, CompareCells> pq;
	vector <Cell> grays;
	vector <Cell> blacks;

	vector <Cell>::iterator itr;


	// add Start Cell to pq and paint it gray
	pq.push(*startingCell);
	grays.push_back(*startingCell);

	// start A* . In our case pq shouldn't be empty because we should find the target before all the 
	// cells are over
	while (!pq.empty())
	{
		// pq is not empty so pick the top Cell
		pcurrent = new Cell(pq.top());
		// check if current is target
		if (pcurrent->getRow() == rooms[index2].getCenterY() && pcurrent->getCol() == rooms[index2].getCenterX())
		{
			RestorePath(pcurrent);
			cout << "The path from room " << index1 << " to room " << index2 << " has been found\n";
			return;
		}
		// remove current from pq and paint it black (remove it from grays)
		pq.pop();
		blacks.push_back(*pcurrent);
		// remove it from grays
		itr = find(grays.begin(), grays.end(), *pcurrent); // Cell must have operator ==
		if (itr == grays.end()) // current cell wasn't found: ERROR
		{
			cout << "Error: current wasn't found in grays for room " << index1 << " to room " << index2 << endl;
			return;
		}
		grays.erase(itr);

		// now check the neighbor cells
		currentRow = pcurrent->getRow();
		currentCol = pcurrent->getCol();
		if (currentRow > 0) // UP
			CheckNeighbor(currentRow - 1, currentCol, pcurrent, pq, grays, blacks);
		if (currentRow < MSZ-1) // DOWN
			CheckNeighbor(currentRow + 1, currentCol, pcurrent, pq, grays, blacks);
		if (currentCol > 0) // LEFT
			CheckNeighbor(currentRow , currentCol-1, pcurrent, pq, grays, blacks);
		if (currentCol < MSZ-1) // RIGHT
			CheckNeighbor(currentRow , currentCol+1, pcurrent, pq, grays, blacks);
	} // while
	// We shouldn't reach this point normally
	cout << "Error: PQ is empty for room " << index1 << " to room " << index2 << endl;
}

void DigPassages()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
		for (j = i + 1; j < NUM_ROOMS; j++)
			DigPath(i, j);
}

void CreateVisibilityMap()
{
	pg->SimulateVisibility(maze, visibility_map);
}

void CreateSecurityMap()
{
	int num_simulations = 500;
	double damage = 0.001;
	int i;
	Grenade* g;

	for (i = 0; i < num_simulations; i++)
	{
		g = new Grenade(rand() % MSZ, rand() % MSZ);
		g->SimulateExplosion(maze, security_map, damage);
	}

}


void ShowMaze() 
{
	int i, j;
	double d, v;
	for(i=0;i<MSZ;i++)
		for (j = 0; j < MSZ; j++)
		{
			// set color for cell (i,j)
			switch (maze[i][j]) 
			{
			case WALL:
				glColor3d(0.5, 0, 0); // dark red
				break;
			case SPACE:
				d = security_map[i][j];
				v = visibility_map[i][j];
				glColor3d(1-d-0.5*v, 1-d, 1-d-0.5*v); // white - security map value - visibility map value
				break;
			case AMMO:
				glColor3d(0, 1, 0);
				break;
			case HP:
				glColor3d(1, 0, 1);
				break;
			case WHITE:
				glColor3d(0.5, 0.5, 0.5);
				break;
			case BLACK:
				glColor3d(0, 0, 0);
				break;
			case GRAY:
				glColor3d(1, 1, 1);
				break;
			}// switch
			// now show the cell as plygon (square)
			glBegin(GL_POLYGON);
			glVertex2d(j, i); // left-bottom corner
			glVertex2d(j, i+1); // left-top corner
			glVertex2d(j+1, i+1); // right-top corner
			glVertex2d(j+1, i); // right-bottom corner
			glEnd();
		}// for
}


void ShowStartAndTarget()
{
	int i, j;
	// start
	i = rooms[r1].getCenterY();
	j = rooms[r1].getCenterX();
	glColor3d(0, 0, 1); // blue
	glBegin(GL_POLYGON);
	glVertex2d(j, i); // left-bottom corner
	glVertex2d(j, i + 1); // left-top corner
	glVertex2d(j + 1, i + 1); // right-top corner
	glVertex2d(j + 1, i); // right-bottom corner
	glEnd();

	// target
	i = rooms[r2].getCenterY();
	j = rooms[r2].getCenterX();
	glColor3d(1, 0.7, 0); // orange
	glBegin(GL_POLYGON);
	glVertex2d(j, i); // left-bottom corner
	glVertex2d(j, i + 1); // left-top corner
	glVertex2d(j + 1, i + 1); // right-top corner
	glVertex2d(j + 1, i); // right-bottom corner
	glEnd();

}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	
	ShowMaze();
	if (underConstruction)
		ShowStartAndTarget();

	for (int i = 0; i < 2; i++)
	{
		if (pWhiteBullets[i] != nullptr)
			pWhiteBullets[i]->show();

		if (pBlackBullets[i] != nullptr)
			pBlackBullets[i]->show();

	}
	if (pg != nullptr)
		pg->show();

	glutSwapBuffers(); // show all
}

void moveSoldiers()
{
	for (int i = 0; i < NUM_OF_SOLDIERS; i++)
	{
		while(blackTeam[i]->HasPath() == false)
			blackTeam[i]->CalcMove(maze);
			
		Sleep(25);
		blackTeam[i]->Move(maze);

		while (whiteTeam[i]->HasPath() == false)
			whiteTeam[i]->CalcMove(maze);

		Sleep(25);
		whiteTeam[i]->Move(maze);

	}

	//for (int i = 0; i < NUM_OF_SOLDIERS - 1; i++)
	//{
	//	int fire = rand() % 100;
	//	if (fire > 85)
	//	{
	//	
	//	}
	//}

}

void idle()
{
	if (underConstruction)
	{
		DigPath(r1, r2);
		// prepare for the next A*
		r2++;
		if (r2 >= NUM_ROOMS)
		{
			r1++;
			r2 = r1 + 1;
			if (r1 + 1 >= NUM_ROOMS)
			{
				underConstruction = false;

		
			}
		}

	}
	// bullet

	if (pg != nullptr && pg->getIsExploded())
		pg->Exploding(maze);
	if (!underConstruction)
	{
		moveSoldiers();
		for (int i = 0; i < 2; i++)
		{
			//Fighter* f = dynamic_cast<Fighter*>(whiteTeam[i]);
			//f->FireBullet(maze);
			if (pWhiteBullets[i] == nullptr || !pWhiteBullets[i]->getIsMoving())
			{
				pWhiteBullets[i] = new Bullet(whiteTeam[i]->getCol(), whiteTeam[i]->getRow(), (rand() % 360) * 3.14 / 180);
				pWhiteBullets[i]->Fire();
			}
			if (pWhiteBullets[i] != nullptr && pWhiteBullets[i]->getIsMoving())
				pWhiteBullets[i]->Move(maze);

			//Fighter* f = dynamic_cast<Fighter*>(blackTeam[i]);
			if (pBlackBullets[i] == nullptr || !pBlackBullets[i]->getIsMoving())
			{
				pBlackBullets[i] = new Bullet(blackTeam[i]->getCol(), blackTeam[i]->getRow(), (rand() % 360) * 3.14 / 180);
				pBlackBullets[i]->Fire();
			}
			if (pBlackBullets[i] != nullptr && pBlackBullets[i]->getIsMoving())
				pBlackBullets[i]->Move(maze);

		}

	}
	glutPostRedisplay(); // indirect call to refresh function (display)
}

void menu(int choice)
{

	if (choice == 2) // grenade
	{
		if (pg != nullptr)
		{
			pg->Explode();
		}
	}
	else if (choice == 3) // create security map
	{
		CreateSecurityMap();
	}
	else if (choice == 4) // create visibility map
	{
		CreateVisibilityMap();
	}

}

// x and y are in pixels
//void mouse(int button, int state, int x, int y)
//{
//	if (pb == nullptr && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		double cx, cy;
//
//		cx = MSZ * x / (double)W;
//		cy = MSZ * (H-y) / (double)H;
//
//		 set x,y and direction angle
//		pb = new Bullet(cx, cy, (rand() % 360)*3.14 / 180);
//		pg = new Grenade(cx, cy);
//	}
//}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // double buffering for animation
	glutInitWindowSize(W, H);
	glutInitWindowPosition(700, 100);
	glutCreateWindow("Dungeons Example");

	glutDisplayFunc(display); // sets display function as window refresh function
	glutIdleFunc(idle); // runs all the time when nothing happens
	//glutMouseFunc(mouse);


	// menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Fire Bullet", 1);
	glutAddMenuEntry("Throw Grenade", 2);
	glutAddMenuEntry("Create Security Map", 3);
	glutAddMenuEntry("Create Visibility Map", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	init();

	glutMainLoop(); // starts window queue of events
}