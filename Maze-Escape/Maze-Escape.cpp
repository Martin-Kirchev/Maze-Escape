
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;


const unsigned int slow_seconds = 2;


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

void printMapStatus(unsigned int iterator, string difficulty) {

	cout << "Level: " << iterator << endl;
	cout << "Difficulty: " << difficulty << endl;
}

void printPlayerStats(unsigned int lives, unsigned int coins, unsigned int keys) {

	cout << "Lives: " << lives << endl;
	cout << "Coins: " << coins << endl;
	cout << "Keys: " << ((keys == 0) ? "Not Found" : to_string(keys)) << endl;
}

void printMaze(unsigned int playerRow, unsigned int playerColumn, unsigned int mapRows, unsigned int mapColumns, char maze[20][20]) {

	for (size_t row = 0; row < mapRows; row++)
	{
		for (size_t colm = 0; colm < mapColumns; colm++)
		{

			if (row == playerRow && colm == playerColumn) {

				cout << "!";
				continue;
			}

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

	delay(slow_seconds);
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

	if (*playerMapsPointer == "") {

		cout << "NO_MAPS_FOUND";
		return "EMPTY_LIST";
	}

	printNewLine();
	cout << "Maze loading successful!" << endl;

	delay(slow_seconds);
	return mapName;
}

void loadMapInfo(string& mapName, string& difficulty, unsigned int& mapNumber, string& size) {

	if (mapName == "EMPTY_LIST") {

		difficulty = "normal";
		mapNumber = getRandomNumber(6);
		size = "12x15";

		string delimiter = "-";
		mapName = difficulty + delimiter + to_string(mapNumber) + delimiter + size;
	}
	else {

		string mapComponents[3];
		split(mapName, '-', mapComponents);

		difficulty = mapComponents[0];
		mapNumber = stoi(mapComponents[1]);
		size = mapComponents[2];
	}
}

void clearSymbol(unsigned int row, unsigned int col, char maze[20][20]) {

	maze[row][col] = ' ';
}

string findPlayer(unsigned int rows, unsigned int columns, char maze[20][20]) {

	for (size_t row = 0; row < rows; row++)
	{
		for (size_t colm = 0; colm < columns; colm++)
		{
			if (maze[row][colm] == '@') {

				clearSymbol(row, colm, maze);

				return to_string(row) + "x" + to_string(colm);
			}
		}
	}

}

boolean playerComandValidator(string command) {

	return (command == "W") || (command == "A") || (command == "S") || (command == "D") || (command == "ESC");
}

boolean playerMovementValidator(unsigned int playerRow, unsigned int playerCol, unsigned int mapRow, unsigned int mapCol) {

	return (playerRow >= 0 && playerRow < mapRow) && (playerCol >= 0 && playerCol < mapCol);
}

void teleportPlayer(unsigned int& playerRow, unsigned int& playerCol, unsigned int mapRow, unsigned int mapCol, char maze[20][20]) {

	int row = playerRow;
	int col = playerCol + 1;

	while (true) {

		while (row < mapRow) {

			while (col < mapCol) {

				if (maze[row][col] == '%' /*&& row != playerRow && col != playerCol*/) {

					playerRow = row;
					playerCol = col;

					return;
				}

				col++;
			}

			row++;
			col = 0;
		}

		row = 0;
	}
}

boolean OpenChest(void (*reverseMove)(unsigned int&, unsigned int&), unsigned int& playerRow, unsigned int& playerCol,
	unsigned int& keys, unsigned int& coins, char maze[20][20]) {

	if (keys > 0) {

		int randomNumber = getRandomNumber(10);
		keys--;
		clearSymbol(playerRow, playerCol, maze);

		cout << "Opening chest..." << endl;
		cout << "+" << randomNumber << " coins" << endl;
		return true;

	}
	else {

		reverseMove(playerRow, playerCol);

		cout << "You don't have any keys!" << endl;
		return false;
	}
}

void moveUp(unsigned int& playerRow, unsigned int& playerCol) {

	playerRow--;
}

void moveLeft(unsigned int& playerRow, unsigned int& playerCol) {

	playerCol--;
}

void moveRight(unsigned int& playerRow, unsigned int& playerCol) {

	playerCol++;
}

void moveDown(unsigned int& playerRow, unsigned int& playerCol) {

	playerRow++;
}

void goToMenu() {

}

void symbolOperations(unsigned int& lives, unsigned int& coins, unsigned int& keys,
	void (*reverseMove)(unsigned int&, unsigned int&),
	unsigned int& playerRow, unsigned int& playerCol,
	unsigned int& mapRow, unsigned int& mapCol, char maze[20][20]) {

	char currentSymbol = maze[playerRow][playerCol];

	if (currentSymbol == '#') {

		cout << "You can't go through the wall!" << endl;
		cout << "-1 live" << endl;

		reverseMove(playerRow, playerCol);
		lives--;

	}
	else if (currentSymbol == 'X') {



		OpenChest(reverseMove, playerRow, playerCol, keys, coins, maze);

	}
	else if (currentSymbol == '%') {

		cout << "Teleporting player..." << endl;

		teleportPlayer(playerRow, playerCol, mapRow, mapCol, maze);

	}
	else if (currentSymbol == 'C') {

		cout << "You found a coin!" << endl;
		cout << "+1 coin" << endl;

		clearSymbol(playerRow, playerCol, maze);
		coins++;

	}
	else if (currentSymbol == '&') {

		cout << "You found a key!" << endl;
		cout << "+1 key" << endl;

		clearSymbol(playerRow, playerCol, maze);
		keys++;
	}
	else {

		cout << "Nothing happend!" << endl;
	}

}

void movePlayer(unsigned int& lives, unsigned int& coins, unsigned int& keys,
	string& playerName, unsigned int& playerRow, unsigned int& playerCol,
	unsigned int& mapRow, unsigned int& mapCol, char maze[20][20]) {

	cout << "Type on of the following commands:" << endl;
	cout << "W/A/S/D/ESC" << endl;

	string command;
	cin >> command;
	toUpper(command);

	while (!playerComandValidator(command)) {

		cout << "Player command \"" << command << "\" is invalid!" << endl;
		cin >> command;
		toUpper(command);
	}

	printNewLine();
	cout << "Message: ";

	if (command == "W") {

		if (playerMovementValidator(playerRow - 1, playerCol, mapRow, mapCol)) {

			moveUp(playerRow, playerCol);
			symbolOperations(lives, coins, keys, &moveDown, playerRow, playerCol, mapRow, mapCol, maze);
		}
	}
	else if (command == "A") {

		if (playerMovementValidator(playerRow, playerCol - 1, mapRow, mapCol)) {

			moveLeft(playerRow, playerCol);
			symbolOperations(lives, coins, keys, &moveRight, playerRow, playerCol, mapRow, mapCol, maze);
		}
	}
	else if (command == "S") {

		if (playerMovementValidator(playerRow + 1, playerCol, mapRow, mapCol)) {

			moveDown(playerRow, playerCol);
			symbolOperations(lives, coins, keys, &moveUp, playerRow, playerCol, mapRow, mapCol, maze);
		}
	}
	else if (command == "D") {

		if (playerMovementValidator(playerRow, playerCol + 1, mapRow, mapCol)) {

			moveRight(playerRow, playerCol);
			symbolOperations(lives, coins, keys, &moveLeft, playerRow, playerCol, mapRow, mapCol, maze);
		}
	}
	else if (command == "ESC") {

		goToMenu();
	}

}

string checkForGameEnd(unsigned int& lives, unsigned int& mapRows, unsigned int& mapCols, char maze[20][20]) {

	if (lives == 0) {

		return "NO_LIVES";
	}

	for (size_t row = 0; row < mapRows; row++)
	{
		for (size_t colm = 0; colm < mapCols; colm++)
		{
			if (maze[row][colm] == 'X') {

				return "CHEST_EXIST";
			}
		}
	}

	return "NO_CHEST_EXIST";
}

void MazeEscape() {

	string playerName = startPlayerSystem();
	clearConsole();

	string mapName = selectLevel(playerName);
	clearConsole();


	const int maxRow = 20;
	const int maxCol = 20;
	char maze[maxRow][maxCol];

	int moves = 0;

	while (true) {

		string mapDifficulty;
		unsigned int mapNumber;
		string mapSize;

		loadMapInfo(mapName, mapDifficulty, mapNumber, mapSize);
		createMap(mapDifficulty, mapNumber, mapSize, maze);

		string cordinates1[2];
		string cordinates2[2];

		split(mapSize, 'x', cordinates1);
		unsigned int mapRow = stoi(cordinates1[0]);
		unsigned int mapCol = stoi(cordinates1[1]);

		split(findPlayer(mapRow, mapCol, maze), 'x', cordinates2);
		unsigned int playerRow = stoi(cordinates2[0]);
		unsigned int playerCol = stoi(cordinates2[1]);

		int mapIterator = 1;
		unsigned int lives = 10;
		unsigned int coins = 0;
		unsigned int keys = 0;

		while (true) {

			printMapStatus(mapIterator, mapDifficulty);
			printNewLine();
			printPlayerStats(lives, coins, keys);
			printNewLine();
			printMaze(playerRow, playerCol, mapRow, mapCol, maze);
			printNewLine();

			movePlayer(lives, coins, keys, playerName, playerRow, playerCol, mapRow, mapCol, maze);

			/*if (checkForGameEnd(lives, mapRow, mapCol, maze) == "CHEST_EXIST") {

				moves++;
				continue;

			}
			else*/ 
			if (checkForGameEnd(lives, mapRow, mapCol, maze) == "NO_CHEST_EXIST") {

				clearConsole();
				return;

			}
			else if (checkForGameEnd(lives, mapRow, mapCol, maze) == "NO_LIVES") {

				moves = 0;
				break;
			}

			moves++;

			delay(slow_seconds);
			clearConsole();
		}
	}
}

int main()
{

	MazeEscape();
}

