
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;

boolean playerExist(string name) {

	string directory = "D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + name + ".txt";

	fstream playerFile(directory);

	return playerFile.is_open();
}

void createPlayerFile(string name) {

	while (playerExist(name)) {

		cout << "Player \"" <<  name  << "\" already exists!" << endl;
		cin >> name;
	}

	ofstream playerFile("D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + name + ".txt");
	playerFile.close();
}

void createPlayerData(string name) {

	fstream playerFile("D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + name + ".txt", ios::app);

	playerFile << "Lives: 0" << endl;
	playerFile << "Coins: 0" << endl;
	playerFile << "Maps:" << endl;
}

void createPlayer(string name) {

	createPlayerFile(name);
	createPlayerData(name);
}


void printMaze(int rows, int columns, char maze[20][20]) {

	for (size_t row = 0; row < rows; row++)
	{
		for (size_t colm = 0; colm < columns; colm++)
		{

			cout << maze[row][colm];
		}

		cout << endl;
	}
}

int getRandomNumber(int max) {

	srand(time(0));

	return (rand() % max) + 1;
}

string getMapDirectory(string difficulty, int number, string size) {

	string directory = "D:\\Workspace\\Maze-Escape\\Maze-Escape\\Maps\\";

	string delimiter = "-";
	string fileType = ".txt";

	directory = directory + difficulty + delimiter + to_string(number) + delimiter + size + fileType;

	return directory;
}

void copyMapToArray(fstream& mazeFile, char maze[20][20]) {

	int i = 0;
	string line;

	while (getline(mazeFile, line)) {

		strcpy_s(maze[i++], line.c_str());
	}
}

void createMap(string difficulty, int randomNumber, string size, char maze[20][20]) {

	fstream mazeFile(getMapDirectory(difficulty, randomNumber, size), ios::in);

	if (!mazeFile.is_open()) {

		cout << "IDK";

		mazeFile.close();
		return;
	}

	copyMapToArray(mazeFile, maze);

	mazeFile.close();
}

int main()
{

	cout << "Login/Register" << endl;

	string command;

	cin >> command;

	createPlayer(command);

	const int row = 20;
	const int col = 20;

	char maze[row][col];

	string difficulty;
	int randomNumber = getRandomNumber(6);
	string size;

	cin >> difficulty >> size;

	createMap(difficulty, randomNumber, size, maze);
	printMaze(12, 15, maze);


}

