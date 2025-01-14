
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;



void clearConsole() {

	system("cls");
}

void printNewLine() {

	cout << endl;
}

int getRandomNumber(int max) {

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

string toUpper(string word) {

	string upperWord;
	char symbols[20];

	strcpy_s(symbols, word.c_str());

	int iterator = 0;

	while (symbols[iterator] != NULL) {

		upperWord += toupper(symbols[iterator++]);
	}

	return upperWord;
}

boolean nameValidator(string playerName) {

	int nameLength = playerName.length();

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

	playerFile << "Lives: 0" << endl;
	playerFile << "Coins: 0" << endl;
	playerFile << "Keys: 0" << endl;
	playerFile << "Completed Maps:" << endl;
	playerFile << "Unfinished Map:" << endl;
}

void registerPlayer(string playerName) {

	while (!nameValidator(playerName)) {

		cout << "Player name \"" << playerName << "\" must have between [1, 50] symbols!" << endl;
		cin >> playerName;
	}

	while (playerExist(playerName)) {

		cout << "Player \"" << playerName << "\" already exists!" << endl;
		cin >> playerName;
	}

	createPlayerFile(playerName);
	createPlayerData(playerName);

	cout << "Register successful!" << endl;
}

void loginPlayer(string playerName) {

	while (!nameValidator(playerName)) {

		cout << "Player name \"" << playerName << "\" must have between [1, 50] symbols!" << endl;
		cin >> playerName;
	}
	
	while (!playerExist(playerName)) {

		cout << "Player \"" << playerName << "\" doesn't exists!" << endl;
		cin >> playerName;
	}

	cout << "Login successful!" << endl;
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
	command = toUpper(command);

	while (!playerSystemCommandValidator(command)) {

		cout << "Command \"" << command << "\" is invalid!" << endl;
		cin >> command;
		command = toUpper(command);
	}

	string playerName;
	cin >> playerName;

	if ((command == "LOGIN")) {

		loginPlayer(playerName);
	}
	else {

		registerPlayer(playerName);
	}

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

string getMapName(int mapNumber, string* mapListPointer) {

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

	cout << "List of completed maps:" << endl;
	printListOfMaps(playerMapsPointer);

	cout << "Type the number of the map you want to play!" << endl;
	int mapNumber;
	cin >> mapNumber;

	mapName = getMapName(mapNumber, playerMapsPointer);

	while (mapName == "WRONG_NUMBER_INPUT") {

		cout << "Number \"" << mapNumber << "\" is invalid!" << endl;
		mapName = getMapName(mapNumber, playerMapsPointer);
	}

	return mapName;
}

string levelSelector(string playerName) {

	string* playerMapsPointer = loadPlayerMaps(playerName);

	if (*playerMapsPointer == "") {

		return "EMPTY_LIST";
	}

	string command;

	cout << "Would you like to continue or select a previous map?" << endl;;
	cout << "Continue/Select" << endl;;
	cin >> command;
	command = toUpper(command);

	while (!levelSelectorCommandValidator(command)) {

		cout << "Command \"" << command << "\" is invalid!" << endl;
		cin >> command;
		command = toUpper(command);
	}

	string mapName;

	if ((command == "CONTINUE")) {

		mapName = getLastPlayedPlayerMap(playerName);
	}
	else {

		mapName = getComplietedMaps(playerMapsPointer);
	}

	return mapName;
}

int main()
{

	string playerName = startPlayerSystem();
	clearConsole();

	string mapName = levelSelector(playerName);
	clearConsole();

	const int row = 20;
	const int col = 20;
	char maze[row][col];

	string difficulty;
	int mapNumber;
	string size;

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
	}


	createMap(difficulty, mapNumber, size, maze);
	printMaze(12, 15, maze);


}

