
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

void split(string word, char delimiter, string array[10]) {

	fill(array, array + 10, "");

	int wordLength = word.size();
	int arrayIterator = 0;

	for (size_t i = 0; i < wordLength; i++)
	{
		char currentLetter = (char)word.at(i);

		if (currentLetter == delimiter) {

			arrayIterator++;
			continue;
		}

		array[arrayIterator] += currentLetter;
	}
}

void toUpper(string& word) {

	string uppedWord;
	int wordLength = word.size();

	for (size_t i = 0; i < wordLength; i++)
	{
		char currentLetter = (char) word.at(i);
		uppedWord += toupper(currentLetter);
	}

	word = uppedWord;
}

boolean playerSystemCommandValidator(string command) {

	return (command == "LOGIN") || (command == "REGISTER") || (command == "EXIT");
}

boolean playerNameValidator(string playerName) {

	int nameLength = playerName.length();

	return (nameLength >= 1) && (nameLength <= 50);
}

boolean playerExists(string playerName) {

	fstream playerFile("Players\\" + playerName + ".txt");

	return playerFile.is_open();
}

boolean levelSelectorCommandValidator(string command) {

	return (command == "CONTINUE") || (command == "SELECT");
}


boolean mapNumberValidator(int mapNumber, string  complietedMaps[10]) {

	return mapNumber >= 0 && mapNumber <= 9 && complietedMaps[mapNumber] != "";
}

boolean playerComandValidator(string command) {

	return (command == "W") || (command == "A") || (command == "S") || (command == "D") || (command == "ESC");
}

boolean playerMovementValidator(unsigned int playerRow, unsigned int playerCol, unsigned int mapRow, unsigned int mapCol) {

	return (playerRow >= 0 && playerRow < mapRow) && (playerCol >= 0 && playerCol < mapCol);
}

boolean menuComandValidator(string comand) {

	return (comand == "LOG OUT") || (comand == "EXIT LEVEL") || (comand == "EXIT GAME");
}

void checkPlayerNameLogin(string& playerName) {

	while (!(playerNameValidator(playerName) && playerExists(playerName))) {

		cout << "Player name \"" << playerName << "\" must be between [1, 50] symbols and exist!" << endl;
		cin >> playerName;
		toUpper(playerName);

		clearConsole();
	}
}

void checkPlayerNameRegistration(string& playerName) {

	while (!(playerNameValidator(playerName) && !playerExists(playerName))) {

		cout << "Player name \"" << playerName << "\" must be between [1, 50] symbols and not exist!" << endl;
		cin >> playerName;
		toUpper(playerName);

		clearConsole();
	}
}

void getDifficulty(int mapIterator, string& mapDifficulty) {

	if (mapIterator == 10) {

		clearConsole();
		cout << "Congrats you beat the game!";
		delay(5);
		exit(0);
	}

	if (mapIterator >= 7) {

		mapDifficulty = "nightmare";
	}
	else if (mapIterator >= 4) {

		mapDifficulty = "hard";
	}
	else if (mapIterator >= 0) {

		mapDifficulty = "normal";
	}
}

void getSize(string& mapDifficulty, string& size) {

	if (mapDifficulty == "nightmare") {

		size = "20x20";
	}
	else if (mapDifficulty == "hard") {

		size = "15x15";
	}
	else if (mapDifficulty == "normal") {

		size = "12x15";
	}
}

string getMapDirectory(string difficulty, unsigned int mapNumber, string size, string bonusPlayerName) {

	string delimiter = "-";
	string fileType = ".txt";
	string bonus = (bonusPlayerName == "") ? "" : delimiter + bonusPlayerName;

	string directory = "Maps\\" + difficulty + delimiter + to_string(mapNumber) + delimiter + size + bonus + fileType;

	return directory;
}

string getLastPlayedPlayerMap(string playerName) {

	fstream playerFile("Players\\" + playerName + ".txt", ios::in);
	string line;

	while (getline(playerFile, line)) {

		if (line == "Unfinished Map:") {

			getline(playerFile, line);
			return line;
		}
	}

	return line;
}

void getComplietedMaps(vector<string> playerData, string complietedMaps[10]) {

	boolean canWriteLine = false;
	int iterator = 0;

	for (string line : playerData) {

		if (line == "Unfinished Map:") {

			return;

		}
		else if (canWriteLine) {

			complietedMaps[iterator++] = line;

		}
		else if (line == "Completed Maps:") {

			canWriteLine = true;
		}
	}
}

void getUnfinishedMaps(vector<string> playerData, string& unfinishedMap) {

	boolean canWriteLine = false;

	for (string line : playerData) {

		if (canWriteLine) {

			unfinishedMap = line;
			return;
		}
		else if (line == "Unfinished Map:") {

			canWriteLine = true;
		}
	}
}

int getAllMapsCount(vector<string> playerData) {

	boolean canCount = false;
	int counter = 0;

	for (string line : playerData) {

		if (line == "Unfinished Map:") {

			return counter;

		}
		else if (canCount) {

			counter++;

		}
		else if (line == "Completed Maps:") {

			canCount = true;
		}
	}
}

void getPlayerData(vector<string> playerData) {

	for (size_t i = 0; i < playerData.size(); i++)
	{
		cout << playerData.at(i) << endl;
	}
}

void readPlayerData(string playerName, vector<string>& playerData) {

	fstream playerFile("Players\\" + playerName + ".txt", ios::in);
	string line;

	while (getline(playerFile, line)) {

		playerData.push_back(line);
	}

	playerFile.close();
}

void copyMapToArray(fstream& mazeFile, char maze[21][21]) {

	int iterator = 0;
	string line;

	while (getline(mazeFile, line)) {

		strcpy_s(maze[iterator++], line.c_str());
	}
}

void createPlayerFile(string playerName) {

	fstream playerFile("Players\\" + playerName + ".txt", ios::app);

	playerFile << "Lives:5" << endl;
	playerFile << "Coins:0" << endl;
	playerFile << "Keys:0" << endl;
	playerFile << "Completed Maps:" << endl;
	playerFile << "Unfinished Map:" << endl;

	playerFile.close();
}

void createMap(string difficulty, unsigned int randomNumber, string size, string bonusPlayerName, char maze[21][21]) {

	string directory = getMapDirectory(difficulty, randomNumber, size, bonusPlayerName);

	fstream mazeFile(directory, ios::in);
	copyMapToArray(mazeFile, maze);

	mazeFile.close();
}

void createUncompleteMap(unsigned int playerRow, unsigned int playerCol,
	unsigned int mapRow, unsigned int mapCol, char maze[21][21], string mapName) {

	fstream mapFile("Maps\\" + mapName + ".txt", ios::out);

	for (size_t i1 = 0; i1 < mapRow; i1++)
	{
		for (size_t i2 = 0; i2 < mapCol; i2++)
		{

			if (i1 == playerRow && i2 == playerCol) {

				mapFile << '@';
				continue;
			}

			mapFile << maze[i1][i2];
		}
		mapFile << endl;
	}

	mapFile.close();
}

void deleteLastVectorElement(vector<string>& playerData) {

	playerData.pop_back();
}

void deleteOtherMaps(int mapNumber, vector<string>& playerData) {

	mapNumber += 4;

	while (int playerDataSize = playerData.size() > mapNumber) {

		deleteLastVectorElement(playerData);
	}

	playerData.push_back("Unfinished Map:");
}

void updateUncompletePlayerData(unsigned int lives, unsigned int coins, unsigned int keys,
	string& mapName, vector<string>& playerData, string playerName) {

	for (size_t i = 0; i < playerData.size(); i++)
	{
		if (i == 0) {

			playerData.at(i) = "Lives:" + to_string(lives);
		}
		else if (i == 1) {

			playerData.at(i) = "Coins:" + to_string(coins);
		}
		else if (i == 2) {

			playerData.at(i) = "Keys:" + to_string(keys);
		}
	}

	string array[10];
	split(mapName, '-', array);

	if (array[3] == "") {

		mapName = mapName + '-' + playerName;
	}

	playerData.push_back(mapName);
}

void updateCompletePlayerData(unsigned int lives, unsigned int coins, unsigned int keys,
	string& mapName, vector<string>& playerData) {

	string array[10];
	split(mapName, '-', array);

	if (array[3] != "") {

		mapName = array[0] + '-' + array[1] + '-' + array[2];
	}

	for (size_t i = 0; i < playerData.size(); i++)
	{
		string currentData = playerData.at(i);

		if (i == 0) {

			playerData.at(i) = "Lives:" + to_string(lives);
		}
		else if (i == 1) {

			playerData.at(i) = "Coins:" + to_string(coins);
		}
		else if (i == 2) {

			playerData.at(i) = "Keys:" + to_string(keys);
		}
		else if (currentData == "Unfinished Map:") {

			playerData.insert(playerData.begin() + i, mapName);
			return;
		}
	}
}

void addDataToPlayerProfile(string playerName, vector<string> playerData) {

	fstream playerFile("Players\\" + playerName + ".txt", ios::out);

	for (string line : playerData) {

		playerFile << line << endl;
	}

	playerFile.close();
}

void loadPlayerStats(unsigned int& lives, unsigned int& coins, unsigned int& keys, vector<string> playerData) {

	int iterator = 0;
	string lineData[10];

	split(playerData.at(iterator++), ':', lineData);
	lives = stoi(lineData[1]);

	split(playerData.at(iterator++), ':', lineData);
	coins = stoi(lineData[1]);

	split(playerData.at(iterator++), ':', lineData);
	keys = stoi(lineData[1]);
}

void printMapStatus(unsigned int currentMazeNumber, string difficulty) {

	cout << "Level: " << currentMazeNumber << endl;
	cout << "Difficulty: " << difficulty << endl;
}

void printPlayerStats(unsigned int lives, unsigned int coins, unsigned int keys, string playerName) {

	cout << "Profile: " << playerName << endl;
	printNewLine();

	cout << "Lives: " << lives << endl;
	cout << "Coins: " << coins << endl;
	cout << "Keys: " << ((keys == 0) ? "Not Found" : to_string(keys)) << endl;
}

void printMaze(unsigned int playerRow, unsigned int playerColumn, unsigned int mapRows, unsigned int mapColumns, char maze[21][21]) {

	for (size_t row = 0; row < mapRows; row++)
	{
		for (size_t colm = 0; colm < mapColumns; colm++)
		{

			if (row == playerRow && colm == playerColumn) {

				cout << "@";
				continue;
			}

			cout << maze[row][colm];
		}

		cout << endl;
	}
}

void printListOfMaps(string complietedMaps[10]) {

	for (size_t i = 0; i < 10; i++)
	{
		if (complietedMaps[i] == "") {

			return;
		}

		string splitedMap[10];
		split(complietedMaps[i], '-', splitedMap);

		cout << i + 1 << ". " << splitedMap[0] << endl;
	}
}

void registerPlayer(string& playerName) {

	clearConsole();

	checkPlayerNameRegistration(playerName);
	createPlayerFile(playerName);

	cout << "Register successful!" << endl;
}

void loginPlayer(string playerName) {

	clearConsole();

	checkPlayerNameLogin(playerName);

	cout << "Login successful!" << endl;
}

string startPlayerSystem() {

	cout << "Login/Register/Exit" << endl;

	string command;
	cin >> command;
	toUpper(command);

	clearConsole();

	while (!playerSystemCommandValidator(command)) {

		cout << "Command \"" << command << "\" is invalid! (Login/Register/Exit)" << endl;
		cin >> command;
		toUpper(command);
		clearConsole();
	}

	if (command == "EXIT") {

		exit(0);
	}

	cout << "Enter your nickname:" << endl;

	string playerName;
	cin >> playerName;
	toUpper(playerName);

	if (command == "LOGIN") {

		loginPlayer(playerName);
	}
	else if (command == "REGISTER") {

		registerPlayer(playerName);
	}

	delay(2);
	return playerName;
}

string selectComplietedMap(string complietedMaps[10], vector<string>& playerData) {

	int mapNumber;

	while (true) {

		cout << "List of completed mazes:" << endl;
		printListOfMaps(complietedMaps);
		printNewLine();

		cout << "Type the number of the maze you want to play!" << endl;
		cin >> mapNumber;
		mapNumber -= 1;

		if (!mapNumberValidator(mapNumber, complietedMaps)) {

			clearConsole();
			cout << "Number \"" << mapNumber + 1 << "\" is invalid!" << endl;

			delay(2);
			clearConsole();
			continue;
		}

		break;
	}

	deleteOtherMaps(mapNumber, playerData);

	return complietedMaps[mapNumber];
}

string selectLevel(vector<string>& playerData) {

	string command;
	string mapName;

	string complietedMaps[10];
	getComplietedMaps(playerData, complietedMaps);

	string unfinishedMap;
	getUnfinishedMaps(playerData, unfinishedMap);

	if (unfinishedMap == "" && complietedMaps[0] == "") {

		return "NO_MAP_FOUND";
	}

	while (true) {

		cout << "Would you like to continue or select a previous maze?" << endl;;
		cout << "Continue/Select" << endl;;
		cin >> command;
		toUpper(command);

		clearConsole();

		while (!levelSelectorCommandValidator(command)) {

			cout << "Command \"" << command << "\" is invalid!" << endl;
			cin >> command;
			toUpper(command);
			clearConsole();
		}

		clearConsole();

		if ((command == "CONTINUE")) {

			if (unfinishedMap == "") {

				mapName = "NO_MAP_FOUND";
				break;
			}

			mapName = unfinishedMap;
			deleteLastVectorElement(playerData);
			break;

		}
		else {

			if (complietedMaps[0] == "") {

				cout << "You don't have any complieted maps!" << endl;
				delay(2);
				clearConsole();

				continue;
			}

			mapName = selectComplietedMap(complietedMaps, playerData);
			break;
		}
	}

	clearConsole();
	cout << "Maze loading successful!" << endl;

	delay(2);
	return mapName;
}

void loadMapInfo(string& mapName, string& difficulty, unsigned int& mapNumber, 
	string& size, string& bonusPlayerName, int mapIterator) {

	if (mapName == "NO_MAP_FOUND") {

		getDifficulty(mapIterator, difficulty);
		mapNumber = getRandomNumber(6);
		getSize(difficulty, size);

		string delimiter = "-";
		mapName = difficulty + delimiter + to_string(mapNumber) + delimiter + size;
	}
	else {

		string mapComponents[10];
		split(mapName, '-', mapComponents);

		difficulty = mapComponents[0];
		mapNumber = stoi(mapComponents[1]);
		size = mapComponents[2];
		bonusPlayerName = mapComponents[3];
	}
}

void clearSymbol(unsigned int row, unsigned int col, char maze[21][21]) {

	maze[row][col] = ' ';
}

string findPlayer(unsigned int rows, unsigned int columns, char maze[21][21]) {

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

void teleportPlayer(unsigned int& playerRow, unsigned int& playerCol, unsigned int mapRow, unsigned int mapCol, char maze[21][21]) {

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
	unsigned int& keys, unsigned int& coins, char maze[21][21]) {

	if (keys > 0) {

		int randomNumber = getRandomNumber(10);
		keys--;
		coins += randomNumber;
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

void goToMenu(string& command, int playerRow, unsigned int playerCol, char maze[21][21]) {

	cout << "Type on of the following commands:" << endl;
	cout << "Log out/Exit level/Exit game" << endl;

	string comand1;
	string comand2;
	cin >> comand1 >> comand2;

	string menuComand = comand1 + " " + comand2;
	toUpper(menuComand);

	while (!menuComandValidator(menuComand)) {

		cout << "Menu command \"" << menuComand << "\" is invalid! (Log out/Exit level/Exit game)" << endl;
		cin >> comand1 >> comand2;
		string menuComand = comand1 + " " + comand2;
		toUpper(menuComand);
		clearConsole();
	}

	if ((menuComand == "LOG OUT") || (menuComand == "EXIT LEVEL")) {

		if (maze[playerRow][playerCol] == '%') {

			cout << "You can't save while standing on a teleport!";
			return;
		}
	}

	command = menuComand;
}

void symbolOperations(unsigned int& lives, unsigned int& coins, unsigned int& keys,
	void (*reverseMove)(unsigned int&, unsigned int&),
	unsigned int& playerRow, unsigned int& playerCol,
	unsigned int mapRow, unsigned int mapCol, char maze[21][21]) {

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

string checkForGameEnd(unsigned int lives, unsigned int mapRows, unsigned int mapCols, char maze[21][21], string command) {

	if (lives == 0) {

		return "NO_LIVES";
	}

	int chestCount = 0;

	for (size_t row = 0; row < mapRows; row++)
	{
		for (size_t colm = 0; colm < mapCols; colm++)
		{
			if (maze[row][colm] == 'X') {

				chestCount++;
			}
		}
	}

	if (chestCount == 0) {

		return "NO_CHEST_EXIST";
	}

	if (command == "LOG OUT") {

		return "LOG_OUT";
	}

	if (command == "EXIT LEVEL") {

		return "EXIT_LEVEL";
	}

	if (command == "EXIT GAME") {

		return "EXIT_GAME";
	}

	return "CONTINUE";
}

void movePlayer(unsigned int& lives, unsigned int& coins, unsigned int& keys, 
	unsigned int& playerRow, unsigned int& playerCol,
	unsigned int mapRow, unsigned int mapCol, char maze[21][21], string& mapStatus) {

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

		goToMenu(command, playerRow, playerCol, maze);
	}

	mapStatus = checkForGameEnd(lives, mapRow, mapCol, maze, command);
}

void MazeEscape() {

	while (true) {

		boolean whileIterator1 = true;

		string playerName = startPlayerSystem();
		clearConsole();
		unsigned int lives = 0;
		unsigned int coins = 0;
		unsigned int keys = 0;

		vector<string> playerData;
		readPlayerData(playerName, playerData);

		clearConsole();

		while (whileIterator1) {

			boolean whileIterator2 = true;

			string mapName = selectLevel(playerData);
			int mapIterator = getAllMapsCount(playerData) + 1;
			clearConsole();

			const int maxRow = 21;
			const int maxCol = 21;
			char maze[maxRow][maxCol];

			string mapDifficulty;
			unsigned int mapNumber;
			string mapSize;
			string bonusPlayerName;

			while (whileIterator2) {

				loadMapInfo(mapName, mapDifficulty, mapNumber, mapSize, bonusPlayerName, mapIterator);
				createMap(mapDifficulty, mapNumber, mapSize, bonusPlayerName, maze);

				string cordinates[10];
				loadPlayerStats(lives, coins, keys, playerData);

				split(mapSize, 'x', cordinates);
				unsigned int mapRow = stoi(cordinates[0]);
				unsigned int mapCol = stoi(cordinates[1]);

				split(findPlayer(mapRow, mapCol, maze), 'x', cordinates);
				unsigned int playerRow = stoi(cordinates[0]);
				unsigned int playerCol = stoi(cordinates[1]);

				string mapStatus;

				while (true) {

					printMapStatus(mapIterator, mapDifficulty);
					printNewLine();
					printPlayerStats(lives, coins, keys, playerName);
					printNewLine();
					printMaze(playerRow, playerCol, mapRow, mapCol, maze);
					printNewLine();

					movePlayer(lives, coins, keys, playerRow, playerCol, mapRow, mapCol, maze, mapStatus);

					if (mapStatus == "NO_CHEST_EXIST") {

						updateCompletePlayerData(lives, coins, keys, mapName, playerData);
						addDataToPlayerProfile(playerName, playerData);
						clearConsole();

						mapName = "NO_MAP_FOUND";
						break;
					}
					else if (mapStatus == "NO_LIVES") {

						clearConsole();
						break;
					}
					else if (mapStatus == "LOG_OUT") {

						updateUncompletePlayerData(lives, coins, keys, mapName, playerData, playerName);
						createUncompleteMap(playerRow, playerCol, mapRow, mapCol, maze, mapName);
						addDataToPlayerProfile(playerName, playerData);
						clearConsole();

						whileIterator1 = false;
						whileIterator2 = false;
						break;
					}
					else if (mapStatus == "EXIT_LEVEL") {

						updateUncompletePlayerData(lives, coins, keys, mapName, playerData, playerName);
						createUncompleteMap(playerRow, playerCol, mapRow, mapCol, maze, mapName);
						addDataToPlayerProfile(playerName, playerData);
						clearConsole();

						whileIterator2 = false;
						break;
					}
					else if (mapStatus == "EXIT_GAME") {

						clearConsole();
						return;
					}

					delay(1);
					clearConsole();
				}
			}
		}
	}
}

int main()
{
	MazeEscape();
}
