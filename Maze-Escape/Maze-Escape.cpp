
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;



void delay(unsigned int sec) {

	clock_t ticks1 = clock();
	clock_t ticks2 = ticks1;

	while ((ticks2 / CLOCKS_PER_SEC - ticks1 / CLOCKS_PER_SEC) < sec) {
		ticks2 = clock();
	}
}

void clearConsole() {

	system("cls");
}

void printNewLine() {

	cout << endl;
}

int getRandomNumber(unsigned int max) {

	srand(time(0));

	return (rand() % max) + 1;
}

void split(string word, char delimiter, string array[3]) {

	char charArray[20];
	strcpy_s(charArray, word.c_str());

	int charIterator = 0;
	int stringIterator = 0;

	while (charArray[charIterator] != '\0') {

		char currentSymbol = charArray[charIterator++];

		if (currentSymbol != delimiter) {

			array[stringIterator] += currentSymbol;
		}
		else {

			stringIterator++;
		}
	}
}

void toUpper(string& word) {

	string upperWord;
	char symbols[20];

	strcpy_s(symbols, word.c_str());

	int iterator = 0;

	while (symbols[iterator] != NULL) {

		upperWord += toupper(symbols[iterator++]);
	}

	word = upperWord;
}

boolean playerNameValidator(string playerName) {

	unsigned int nameLength = playerName.length();

	return (nameLength >= 1) && (nameLength <= 50);
}

boolean playerExist(string playerName) {

	string directory = "D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + playerName + ".txt";

	fstream playerFile(directory);

	return playerFile.is_open();
}

void createPlayerFile(string playerName) {

	ofstream playerFile("D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + playerName + ".txt");
	playerFile.close();
}

void createPlayerData(string playerName) {

	fstream playerFile("D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + playerName + ".txt", ios::app);

	playerFile << "Lives: 10" << endl;
	playerFile << "Coins: 0" << endl;
	playerFile << "Keys: 0" << endl;
	playerFile << "Completed Maps:" << endl;
	playerFile << "Unfinished Map:" << endl;
}

void registerPlayer(string playerName) {

	while (!playerNameValidator(playerName)) {

		cout << "Player name \"" << playerName << "\" must have between [1, 50] symbols!" << endl;
		cin >> playerName;
	}

	while (playerExist(playerName)) {

		cout << "Player \"" << playerName << "\" already exists!" << endl;
		cin >> playerName;
	}

	createPlayerFile(playerName);
	createPlayerData(playerName);

	printNewLine();
	cout << "Register successful!" << endl;
}

void loginPlayer(string playerName) {

	while (!playerNameValidator(playerName)) {

		cout << "Player name \"" << playerName << "\" must have between [1, 50] symbols!" << endl;
		cin >> playerName;
	}

	while (!playerExist(playerName)) {

		cout << "Player \"" << playerName << "\" doesn't exists!" << endl;
		cin >> playerName;
	}

	printNewLine();
	cout << "Login successful!" << endl;
}

void printPlayerStats(string playerName) {

	fstream playerFile("D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + playerName + ".txt", ios::in);
	string line;

	while (getline(playerFile, line)) {

		if (line == "Completed Maps:") {

			return;
		}

		cout << line << endl;
	}
}


void printMaze(unsigned int rows, unsigned int columns, char maze[20][20]) {

	for (size_t row = 0; row < rows; row++)
	{
		for (size_t colm = 0; colm < columns; colm++)
		{
			cout << maze[row][colm];
		}

		cout << endl;
	}
}

string getMapDirectory(string difficulty, unsigned int number, string size) {

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

void createMap(string difficulty, unsigned int randomNumber, string size, char maze[20][20]) {

	fstream mazeFile(getMapDirectory(difficulty, randomNumber, size), ios::in);
	copyMapToArray(mazeFile, maze);

	mazeFile.close();
}

string* loadPlayerMaps(string playerName) {

	string* mapArray = new string[10];

	fstream playerFile("D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + playerName + ".txt", ios::in);
	string line;
	boolean validLine = false;

	int iterator = 0;

	while (getline(playerFile, line)) {

		if (line == "Unfinished Map:") {

			return mapArray;
		}
		else if (line == "Completed Maps:") {

			validLine = true;
			continue;
		}
		else if (validLine) {

			mapArray[iterator++] = line;
		}
	}

	return mapArray;
}

boolean playerSystemCommandValidator(string command) {

	return (command == "LOGIN") || (command == "REGISTER");
}

string startPlayerSystem() {

	cout << "Login/Register" << endl;

	string command;
	cin >> command;
	toUpper(command);

	while (!playerSystemCommandValidator(command)) {

		cout << "Command \"" << command << "\" is invalid!" << endl;
		cin >> command;
		toUpper(command);
	}

	printNewLine();
	cout << "Enter your nickname:" << endl;

	string playerName;
	cin >> playerName;

	if ((command == "LOGIN")) {

		loginPlayer(playerName);
	}
	else {

		registerPlayer(playerName);
	}

	delay(5);
	return playerName;
}

boolean levelSelectorCommandValidator(string command) {

	return (command == "CONTINUE") || (command == "SELECT");
}

void printListOfMaps(string* mapListPointer) {

	int iterator = 1;

	while (*mapListPointer != "") {

		string array[3];
		split(*mapListPointer, '-', array);

		cout << iterator++ << ". " << array[0] << endl;
		mapListPointer++;
	}
}

string getMapName(unsigned int mapNumber, string* mapListPointer) {

	int iterator = 1;

	while (*mapListPointer != "") {

		if (iterator == mapNumber) {

			return *mapListPointer;
		}

		iterator++;
	}

	return "WRONG_NUMBER_INPUT";
}

string getLastPlayedPlayerMap(string playerName) {

	fstream playerFile("D:\\Workspace\\Maze-Escape\\Maze-Escape\\Players\\" + playerName + ".txt", ios::in);
	string line;

	while (getline(playerFile, line)) {

		if (line == "Unfinished Map:") {

			getline(playerFile, line);
			return line;
		}
	}

	return line;
}

string getComplietedMaps(string* playerMapsPointer) {

	string mapName;

	cout << "List of completed mazes:" << endl;
	printListOfMaps(playerMapsPointer);
	printNewLine();

	cout << "Type the number of the maze you want to play!" << endl;
	int mapNumber;
	cin >> mapNumber;

	mapName = getMapName(mapNumber, playerMapsPointer);

	while (mapName == "WRONG_NUMBER_INPUT") {

		cout << "Number \"" << mapNumber << "\" is invalid!" << endl;
		mapName = getMapName(mapNumber, playerMapsPointer);
	}

	return mapName;
}

string selectLevel(string playerName) {

	string* playerMapsPointer = loadPlayerMaps(playerName);

	if (*playerMapsPointer == "") {

		return "EMPTY_LIST";
	}

	string command;

	cout << "Would you like to continue or select a previous maze?" << endl;;
	cout << "Continue/Select" << endl;;
	cin >> command;
	toUpper(command);

	while (!levelSelectorCommandValidator(command)) {

		cout << "Command \"" << command << "\" is invalid!" << endl;
		cin >> command;
		toUpper(command);
	}

	printNewLine();
	string mapName;

	if ((command == "CONTINUE")) {

		mapName = getLastPlayedPlayerMap(playerName);
	}
	else {

		mapName = getComplietedMaps(playerMapsPointer);
	}

	printNewLine();
	cout << "Maze loading successful!" << endl;

	delay(5);
	return mapName;
}

void loadMapInfo(string mapName, string& difficulty, unsigned int& mapNumber, string& size) {

	if (mapName == "EMPTY_LIST") {

		difficulty = "normal";
		mapNumber = getRandomNumber(6);
		size = "12x15";
	}
	else {

		string mapComponents[3];
		split(mapName, '-', mapComponents);

		difficulty = mapComponents[0];
		mapNumber = stoi(mapComponents[1]);
		size = mapComponents[2];

		delete mapComponents;
	}
}

string findPlayer(unsigned int rows, unsigned int columns, char maze[20][20]) {

	for (size_t row = 0; row < rows; row++)
	{
		for (size_t colm = 0; colm < columns; colm++)
		{
			if (maze[row][colm] == '@') {

				return (row + "x" + colm);
			}
		}
	}
}

boolean playerComandValidator(string command) {

	return (command == "W") || (command == "A") || (command == "S") || (command == "D") || (command == "ESC");
}

boolean playerMovementdValidator(unsigned int playerRow, unsigned int playerCol, unsigned int mapRow, unsigned int mapCol) {

	return (playerRow >= 0 && playerRow < mapRow) && (playerCol >= 0 && playerCol < mapCol);
}

void teleportPlayer(unsigned int playerRow, unsigned int playerCol, unsigned int mapRow, unsigned int mapCol, char maze[20][20]) {

	boolean getFirstTeleport = false;

	for (size_t i1 = 0; i1 < mapRow; i1++)
	{
		for (size_t i2 = 0; i2 < mapCol; i2++)
		{

			if ((maze[i1][i2] == '%') && (i1 != playerRow) && (i2 != playerCol)) {

				if (getFirstTeleport || (i1 >= playerRow && i2 >= playerCol)) {

					playerRow = i1;
					playerCol = i2;

					return;
				}
			}

			if ((i1 == mapRow - 1) && (i2 == mapCol - 1)) {

				i1 = 0;
				i2 = 0;
				getFirstTeleport = true;
			}
		}
	}
}

void moveUp(unsigned int playerRow) {

	playerRow--;
}

void moveLeft(unsigned int playerCol) {

	playerCol--;
}

void moveRight(unsigned int playerCol) {

	playerCol++;
}

void moveDown(unsigned int playerRow) {

	playerRow++;
}

void movePlayer(string& playerName, unsigned int& playerRow, unsigned int& playerCol, unsigned int& mapRow, unsigned int& mapCol, char maze[20][20]) {

	unsigned int lives = 0;
	unsigned int coins = 0;
	unsigned int keys = 0;

	string command;
	cin >> command;
	toUpper(command);

	cout << "W/A/S/D/ESC" << endl;

	while (!playerSystemCommandValidator(command)) {

		cout << "Player command \"" << command << "\" is invalid!" << endl;
		cin >> command;
		toUpper(command);
	}

	if (command == "W") {

		if (playerMovementdValidator(playerRow - 1, playerCol, mapRow, mapCol)) {

			char nextMove = maze[playerRow - 1][playerCol];

			if (nextMove == '#') {

				cout << "";
				lives--;
				return;
			}

			if (nextMove == 'X') {

				//tryToOpenChest(playerRow, playerCol);
				return;
			}

			moveUp(playerRow);

			if (nextMove == 'C') {

				coins++;
				cout << "";
			}
			else if (nextMove == '&') {

				keys++;
				cout << "";
			}
			else if (nextMove == '%') {

				teleportPlayer(playerRow, playerCol, mapRow, mapCol, maze);
				cout << "";
			}
		}
	}
	else if (command == "A") {

		if (playerMovementdValidator(playerRow, playerCol - 1, mapRow, mapCol)) {




		}
	}
	else if (command == "S") {

		if (playerMovementdValidator(playerRow + 1, playerCol, mapRow, mapCol)) {


		}

	}
	else if (command == "D") {

		if (playerMovementdValidator(playerRow, playerCol + 1, mapRow, mapCol)) {



		}
	}
	else if (command == "ESC") {

		//goToMenu();

	}

}

void MazeEscape() {

	string playerName = startPlayerSystem();
	clearConsole();

	string mapName = selectLevel(playerName);
	clearConsole();


	const int maxRow = 20;
	const int maxCol = 20;
	char maze[maxRow][maxCol];

	string mapDifficulty;
	unsigned int mapNumber;
	string mapSize;

	loadMapInfo(mapName, mapDifficulty, mapNumber, mapSize);
	createMap(mapDifficulty, mapNumber, mapSize, maze);

	string cordinates[2];

	split(mapSize, 'x', cordinates);
	unsigned int mapRow = stoi(cordinates[0]);
	unsigned int mapCol = stoi(cordinates[1]);

	split(findPlayer(mapRow, mapCol, maze), 'x', cordinates);
	unsigned int playerRow = stoi(cordinates[0]);
	unsigned int playerCol = stoi(cordinates[1]);

	delete cordinates;

	int i = 1;

	while (i--) {

		printPlayerStats(playerName);
		printNewLine();
		printMaze(mapRow, mapCol, maze);

		movePlayer(playerName, playerRow, playerCol, mapRow, mapCol, maze);

	}

}

int main()
{

	MazeEscape();

}

