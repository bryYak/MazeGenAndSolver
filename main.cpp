#include <iostream>
#include <stack>
#include <ctime>

using namespace std;

class Box //Stores Info Containing Each Cell of the Maze
{
	public:
		Box(void);
		Box(int, int);
		void visit(void);
		bool hasVisited(void);
		void removeWall(int);
		int getX(void);
		int getY(void);
		bool checkWall(int);

	private: 
		int xCoord;
		int yCoord;
		bool walls[4];
		bool visited;
};

Box::Box(void)
{
	for (int i = 0; i < 4; i++)
		walls[i] = true;
	visited = false;
}

Box::Box(int x, int y)
{
	yCoord = y;
	xCoord = x;
	for (int i = 0; i < 4; i++)
		walls[i] = true;
	visited = false;
}

void Box::visit(void)
{
	visited = true;
}

bool Box::hasVisited(void)
{
	return visited;
}

void Box::removeWall(int direction)
{
	walls[direction] = false;
}

int Box::getX(void)
{
	return xCoord;
}

int Box::getY(void)
{
	return yCoord;
}

bool Box::checkWall(int wall)
{
	return walls[wall];
}



const int WIDTH = 50;
const int HEIGHT = 50;

void recursiveBacktackGeneration(int, int, Box[HEIGHT][WIDTH], stack<Box>);
bool adjacentVisited(int, int, Box[HEIGHT][WIDTH]);
bool validDirection(int, int, Box[HEIGHT][WIDTH], int);

int main()
{
	//Creates a new maze grid of heightxwidth
	Box boxGrid[HEIGHT][WIDTH];

	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			boxGrid[i][j] = Box::Box(i, j);

	stack<Box> stack;//Will be used to store previous positions

	recursiveBacktackGeneration(HEIGHT/2, WIDTH/2, boxGrid, stack);//Maze generating algorithm, box grid stores the wall info and whether its been visited for the maze, and stack stores previous info

	//Draws maze
	for (int i = 0; i < WIDTH; i++)
		cout << " _";
	cout << "\n";

	for (int i = 0; i < HEIGHT; i++)
	{
		cout << "|";
		for (int j = 0; j < WIDTH; j++)
		{
			if (boxGrid[i][j].checkWall(2))
				cout << "_";
			else
				cout << " ";
			if (boxGrid[i][j].checkWall(1))
				cout << "|";
			else
				cout << " ";
		}

		cout << "\n";
	}

	return 0;
}

void recursiveBacktackGeneration(int x, int y, Box boxGrid[HEIGHT][WIDTH], stack<Box> stack)
{
	srand((unsigned) time(0));

	boxGrid[x][y].visit();//Marks the first square on method call as visited
	do{ 

		if (adjacentVisited(x, y, boxGrid)){//Checks if all adjacent blocks have been visited, if so then the program goes back to the previous position, backtracking the maze
		
			//pop the stack, and update x and y values
			x = stack.top().getX();
			y = stack.top().getY();
			stack.pop();

		}
		else{

			int direction;
			int dX; int dY;

			do{

				direction = rand() % 4;//Picks random direction 0=N,1=E,S=2,W=3

			} while (validDirection(x, y, boxGrid, direction)); //Loop breaks if the direction chosen is valid

			//This assigns the change in the coordinate based on chosen direction
			if (direction == 0) {
				dX = -1; dY = 0;
			}
			else if (direction == 1) {
				dX = 0; dY = 1;
			}
			else if (direction == 2) {
				dX = 1; dY = 0;
			}
			else {
				dX = 0; dY = -1;
			}

			//update coordinates for current box, and future box
			boxGrid[x][y].removeWall(direction);
			boxGrid[x + dX][y + dY].removeWall((direction + 2) % 4);
			boxGrid[x + dX][y + dY].visit();//Marks the future box as visited

			stack.push(boxGrid[x][y]);//Adds current position to stack in order to have backtracking

			//Updates coordinates
			x += dX;
			y += dY;
		}

	} while (stack.size() != 0);//Once the program has filled each square and backtracked back to the original starting square, the maze generation is complete
}

bool adjacentVisited(int x, int y, Box boxGrid[HEIGHT][WIDTH])//returns true if every adjacent square has been visited
{
	if (x != 0 && !boxGrid[x - 1][y].hasVisited())
		return false;
	if (x != HEIGHT - 1 && !boxGrid[x + 1][y].hasVisited())
		return false;
	if (y != 0 && !boxGrid[x][y - 1].hasVisited())
		return false;
	if (y != WIDTH - 1 && !boxGrid[x][y + 1].hasVisited())
		return false;
	return true;
}

bool validDirection(int x, int y, Box boxGrid[HEIGHT][WIDTH], int direction)//Returns if box in chosen direction has been visited, and false if it hasn't
{
	if(x != 0 && direction == 0)
		return boxGrid[x - 1][y].hasVisited();
	if (y != WIDTH - 1 && direction == 1)
		return boxGrid[x][y + 1].hasVisited();
	if (x != HEIGHT - 1 && direction == 2)
		return boxGrid[x + 1][y].hasVisited();
	if (y != 0 && direction == 3)
		return boxGrid[x][y - 1].hasVisited();
	return true;
}
