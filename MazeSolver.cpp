#include <iostream>
#include <stack>
#include <ctime>
#include <windows.h>
#include <limits>

//@author Bryan Yakimisky, Last Updated September 2nd, 2020
using namespace std;

const int WIDTH = 50;
const int HEIGHT = 50;

class Box { //Stores Info Containing Each Cell of the Maze

public:
	Box(void); // Default Constructor
	Box(int, int); //Constructor For Coordinates
	void visit(void); //Marks visited as true
	bool hasVisited(void); // Returns value of visited
	void removeWall(int); // Turns value of wall[int] to false
	int getX(void); //Returns xCoord
	int getY(void); //Returns yCoord
	bool checkWall(int); //Returns value of wall[int]
	void unVisit(void); // Sets visited to false
	bool isStart(void); // Sets isStartPoint to true
	bool isEnd(void); // Sets isEndPoint to true
	void isCorrectPath(void); // Sets correctPath to true
	bool getCorrect(void); // Returns getCorrectPath

private:
	int xCoord; //Stores X Coordinate of Box
	int yCoord; //Stores Y Coordinate of Box
	bool walls[4]; //Stores value of wall (if it exists or if it doesn't exist)
	bool visited; //Stores whether box has been visited
	bool isStartPoint; //States whether the box is the start of the maze
	bool isEndPoint; //States whether the box is the end of the maze
	bool correctPath; //States whether the box is a correct path towards end
};

Box::Box(void) {
	for (int i = 0; i < 4; i++)
		walls[i] = true;
	visited = false;
}

Box::Box(int x, int y) {
	yCoord = y;
	xCoord = x;
	for (int i = 0; i < 4; i++)
		walls[i] = true;
	visited = false;
	if (x == 0 && y == 0)
		isStartPoint = true;
	else
		isStartPoint = false;
	if (x == HEIGHT - 1 && y == WIDTH - 1)
		isEndPoint = true;
	else
		isEndPoint = false;
	correctPath = false;
}

void Box::visit(void) {
	visited = true;
}

bool Box::hasVisited(void) {
	return visited;
}

void Box::removeWall(int direction) {
	walls[direction] = false;
}

int Box::getX(void) {
	return xCoord;
}

int Box::getY(void) {
	return yCoord;
}

bool Box::checkWall(int wall) {
	return walls[wall];
}

void Box::unVisit(void) {
	visited = false;
}

bool Box::isStart(void) {
	return isStartPoint;
}

bool Box::isEnd(void) {
	return isEndPoint;
}

void Box::isCorrectPath(void) {
	correctPath = true;
}

bool Box::getCorrect(void) {
	return correctPath;
}




void recursiveBacktrackGeneration(int, int, Box[HEIGHT][WIDTH], stack<Box>);
bool adjacentVisited(int, int, Box[HEIGHT][WIDTH]);
bool validDirection(int, int, Box[HEIGHT][WIDTH], int);

stack<Box> recursiveBacktrackSolver(int, int, Box[HEIGHT][WIDTH], stack<Box>);
bool availableBox(int, int, Box[HEIGHT][WIDTH]);
bool validDirectionSolver(int, int, Box[HEIGHT][WIDTH], int);

int main() {
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	//Creates a new maze grid of heightxwidth
	Box boxGrid[HEIGHT][WIDTH];

	//Initializes box array with box coordinate location
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			boxGrid[i][j] = Box(i, j);

	stack<Box> stack;//Will be used to store previous positions



	recursiveBacktrackGeneration(HEIGHT / 2, WIDTH / 2, boxGrid, stack);//Maze generating algorithm, box grid stores the wall info and whether its been visited for the maze, and stack stores previous info

	//Sets box visited status to false for solving algorithm
	for (int i = 0; i < HEIGHT; i++)
		for (int j = 0; j < WIDTH; j++)
			boxGrid[i][j].unVisit();

	stack = recursiveBacktrackSolver(0, 0, boxGrid, stack);




	while (stack.size() != 1) {//Stack holds only correct path variables, so sets each box (other than start) to correct path
		boxGrid[stack.top().getX()][stack.top().getY()].isCorrectPath();
		stack.pop();
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); //Allows for color in Windows Terminal



	//Draws maze
	for (int i = 0; i < WIDTH; i++)
		cout << " _";
	cout << "\n";

	for (int i = 0; i < HEIGHT; i++) {
		
		SetConsoleTextAttribute(hConsole, 15); //Sets Background Color to Black
		cout << "|";
		
		for (int j = 0; j < WIDTH; j++) {
			
			if (boxGrid[i][j].getCorrect()) //Checks if box is a correct path
				SetConsoleTextAttribute(hConsole, 159); //Sets background color to blue
			else if (boxGrid[i][j].isStart())//checks if box is start
				SetConsoleTextAttribute(hConsole, 207); //Sets background color to red
			else if (boxGrid[i][j].isEnd()) //checks if box is end
				SetConsoleTextAttribute(hConsole, 175); //Sets background color to green
			else
				SetConsoleTextAttribute(hConsole, 15); //Sets background color to black

			if (boxGrid[i][j].checkWall(2)) //Prints walls and empty space
				cout << "_";
			else
				cout << " ";
			if (boxGrid[i][j].checkWall(1)) {
				SetConsoleTextAttribute(hConsole, 15); //Sets background color to black
				cout << "|";
			}
			else {
				cout << " ";
			}

		}

		cout << "\n";
	}

	cout << "\nPress Enter to Close...\n";//End of program
	std::cin.ignore();

	return 0;
}

void recursiveBacktrackGeneration(int x, int y, Box boxGrid[HEIGHT][WIDTH], stack<Box> stack) { //Generates maze using recursive backtrack algorithm
	srand(time(NULL));
	srand(rand());
	boxGrid[x][y].visit();//Marks the first square on method call as visited
	do {

		if (adjacentVisited(x, y, boxGrid)) {//Checks if all adjacent blocks have been visited, if so then the program goes back to the previous position, backtracking the maze

			//pop the stack, and update x and y values
			x = stack.top().getX();
			y = stack.top().getY();
			stack.pop();

		}
		else {

			int direction;
			int dX; int dY;

			do {

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

bool adjacentVisited(int x, int y, Box boxGrid[HEIGHT][WIDTH]) {//returns true if every adjacent square has been visited
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

bool validDirection(int x, int y, Box boxGrid[HEIGHT][WIDTH], int direction) {//Returns if box in chosen direction has been visited, and false if it hasn't
	if (x != 0 && direction == 0)
		return boxGrid[x - 1][y].hasVisited();
	if (y != WIDTH - 1 && direction == 1)
		return boxGrid[x][y + 1].hasVisited();
	if (x != HEIGHT - 1 && direction == 2)
		return boxGrid[x + 1][y].hasVisited();
	if (y != 0 && direction == 3)
		return boxGrid[x][y - 1].hasVisited();
	return true;
}

stack<Box> recursiveBacktrackSolver(int x, int y, Box boxGrid[HEIGHT][WIDTH], stack<Box> stack) { //Solves for correct path of maze
	boxGrid[x][y].visit();

	do {
		if (availableBox(x, y, boxGrid)) { //Checks if available box, if there isn't backtracks to previous box

			x = stack.top().getX();
			y = stack.top().getY();
			stack.pop();

		}
		else {

			int direction;
			int dX;
			int dY;

			for (int i = 0; i < 4; i++) //Finds valid directiion going from priority of N>E>S>W
				if (!validDirectionSolver(x, y, boxGrid, i))
					direction = i;
			
			//Tells what direction the new box is
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

			boxGrid[x + dX][y + dY].visit();//Visits new box

			stack.push(boxGrid[x][y]);//adds current box to stack

			x += dX;
			y += dY;
		}
	} while (!boxGrid[x][y].isEnd());//checks if user is at end square

	return stack;
}

bool availableBox(int x, int y, Box boxGrid[HEIGHT][WIDTH]) {//checks if there are boxes that are unvisited and not blocked by walls
	if (x != 0 && !(boxGrid[x - 1][y].hasVisited() || boxGrid[x][y].checkWall(0)))
		return false;
	if (x != HEIGHT - 1 && !(boxGrid[x + 1][y].hasVisited() || boxGrid[x][y].checkWall(2)))
		return false;
	if (y != 0 && !(boxGrid[x][y - 1].hasVisited() || boxGrid[x][y].checkWall(3)))
		return false;
	if (y != WIDTH - 1 && !(boxGrid[x][y + 1].hasVisited() || boxGrid[x][y].checkWall(1)))
		return false;
	return true;
}

bool validDirectionSolver(int x, int y, Box boxGrid[HEIGHT][WIDTH], int direction) {// Checks if the direction chosen is valid (box unvisited and not blocked by walls
	if (x != 0 && direction == 0 && !boxGrid[x][y].checkWall(direction))
		return boxGrid[x - 1][y].hasVisited();
	if (y != WIDTH - 1 && direction == 1 && !boxGrid[x][y].checkWall(direction))
		return boxGrid[x][y + 1].hasVisited();
	if (x != HEIGHT - 1 && direction == 2 && !boxGrid[x][y].checkWall(direction))
		return boxGrid[x + 1][y].hasVisited();
	if (y != 0 && direction == 3 && !boxGrid[x][y].checkWall(direction))
		return boxGrid[x][y - 1].hasVisited();
	return true;
}
