#include "main.h"
#include "Hero.h"
#include "Creature.h"
#include "Item.h"
Hero	 gHero(1, 1); // hero starting position
std::vector<Creature*> gCreature;
std::vector<Item*> gItem; //the vector for all kinds of item

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Position winPos;
bool gamewin = false;
bool over = false;
int stage = 0;
int starObtain = 0;

// Constent value
const std::string GWALL = "#";
const std::string GNOTHING = " ";

int GWIDTH = 10;
int GHEIGHT = 10;
const double gTimeLog = 0.033;

// Distance
const int gDistance = 4;

std::string** gBoard;

// �w�q���ؿ�J���O�P�����}�Cindex
enum ValidInput
{
	EW = 0,
	ES = 1,
	EA = 2,
	ED = 3,
	ESPACE = 4,
	EESC = 5,
	INVALID,
};

// function declare
void keyUpdate(bool key[]);
bool isPositionValid(Position& loc);
void setupBoard();
void fillBoard();
void mazeWalk(std::string** board);
void mazeStep(std::string** board, Position pos, int width, int height);
void draw(void);
void drawInfo(void);

void update(bool key[]);

void ruleList()
{
	std::cout << "Welcome to Dungeon Prototype..." << std::endl << std::endl;
	std::cout << "This game will have 3 levels, and in each level you must collect 3 STARS(*) in the map to proceed to the deeper dungeon" << std::endl;
	std::cout << "Keep in that you can step on 'T' to level up your hero and 'o' to restore or to heal your hp" << std::endl;
	std::cout << "Value from T and o will be randomized!" << std::endl;
	std::cout << "Once you reach level 5 you can hit and knock the creature one step in exchange of 1 hit of the creature" << std::endl;
	std::cout << "Once you reach level 10 you can knock the creature to the next corner" << std::endl << std::endl;
	std::cout << "Finally you can collect sword '/' to raise your attack for attacking the enemy, but you can only attack the enemy once you have reached level 5!" << std::endl << std::endl;
	std::cout << "Beware of these creatures. They can free roam, so don't stay in one place for too long!" << std::endl;
	std::cout << "Goblin (G) -> Just your random everday life enemy" << std::endl;
	std::cout << "Chaser (C) -> Beware as he sets his eye on you until he hits you" << std::endl;
	std::cout << "Killer (K) -> Some people thinks he is slow but once he hits you he can kill you in no time" << std::endl << std::endl;
	std::cout << "You can kill the creature, but beware, they will respawn in no time!So get away while you can!" << std::endl;
	std::cout << "Attacking creature once = getting hit once by the creature, be wise!" << std::endl;
	std::cout << "Good luck! :)" << std::endl << std::endl;

	system("pause");
	system("cls");
}


int launchDungeon()
{
	bool gKeyState[6];

	ruleList();

	for (int i = 1; i <= 3; i++) // loop game level
	{
		stage = i;
		if (stage == 1)
		{
			std::cout << "Stage 1: ";
			SetConsoleTextAttribute(hConsole, 10);
			std::cout << "Mellow Mead Woods\n";
			SetConsoleTextAttribute(hConsole, 14);
		}
		else if (stage == 2)
		{
			std::cout << "Stage 2: ";
			SetConsoleTextAttribute(hConsole, 11);
			std::cout << "Wraith's Frozen Tundra\n";
			SetConsoleTextAttribute(hConsole, 14);
		}
		else if (stage == 2)
		{
			std::cout << "Stage 3: ";
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "Ozai's Red Hot Bolero\n";
			SetConsoleTextAttribute(hConsole, 14);
		}
		std::cout << "Get ready... Here goes nothing!\n";
		system("pause");
		system("cls");
		gamewin = false;
		gHero.setPos(1, 1);
		starObtain = 0;
		gItem.clear();
		gCreature.clear();
		if (i == 1) // level 1 objects
		{
			GWIDTH = 11;
			GHEIGHT = 11;
			winPos.x = 9;
			winPos.y = 9;
			Goblin goblin1(2 * (rand() % 3) + 1, 2 * (rand() % 3) + 1);
			Goblin goblin2(2 * (rand() % 3) + 1, 2 * (rand() % 3) + 1);
			gCreature.push_back(&goblin1);
			gCreature.push_back(&goblin2);

			EXPTrigger expTrigger1(2 * (rand() % 3) + 1, 2 * (rand() % 3) + 1, 15);
			// Potion potion1(2 * (rand() % 3) + 1, 2 * (rand() % 3) + 1);
			Star star1(2 * (rand() % 3) + 1, 2 * (rand() % 3) + 1);
			gItem.push_back(&expTrigger1);
			// gItem.push_back(&potion1);
			gItem.push_back(&star1);
		}
		else if (i == 2) // level 2 objects
		{
			GWIDTH = 21;
			GHEIGHT = 15;
			winPos.x = 19;
			winPos.y = 13;
			Goblin goblin1(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1);
			Chaser chaser1(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1);
			Killer killer1(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1);
			gCreature.push_back(&goblin1);
			gCreature.push_back(&chaser1);
			gCreature.push_back(&killer1);

			EXPTrigger expTrigger1(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1, 15);
			Potion potion1(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1);
			Potion potion2(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1);
			Star star1(2 * (rand() % 8) + 1, 2 * (rand() %6) + 1);
			Sword sword1(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1, 10);
			gItem.push_back(&expTrigger1);
			gItem.push_back(&potion1);
			gItem.push_back(&potion2);
			gItem.push_back(&sword1);
			gItem.push_back(&star1);
		}
		else if (i == 3) // level 3 objects
		{
			GWIDTH = 41;
			GHEIGHT = 21;
			winPos.x = 39;
			winPos.y = 19;
			Goblin goblin1(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Goblin goblin2(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Goblin goblin3(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Chaser chaser1(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Chaser chaser2(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Killer killer1(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			gCreature.push_back(&goblin1);
			gCreature.push_back(&goblin2);
			gCreature.push_back(&goblin3);
			gCreature.push_back(&chaser1);
			gCreature.push_back(&chaser2);
			gCreature.push_back(&killer1);

			Item expTrigger1(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1, 15);
			Item expTrigger2(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1, 15);
			Item expTrigger3(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1, 15);
			Potion potion1(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Potion potion2(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Potion potion3(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Star star1(2 * (rand() % 19) + 1, 2 * (rand() % 8) + 1);
			Sword sword1(2 * (rand() % 8) + 1, 2 * (rand() % 6) + 1, 10);
			gItem.push_back(&expTrigger1);
			gItem.push_back(&expTrigger2);
			gItem.push_back(&expTrigger3);
			gItem.push_back(&potion1);
			gItem.push_back(&potion2);
			gItem.push_back(&potion3);
			gItem.push_back(&sword1);
			gItem.push_back(&star1);
		}



		fillBoard();

		draw();
		drawInfo();

		clock_t startT, endT;
		startT = clock();
		endT = clock();

		keyUpdate(gKeyState);

		while (!gKeyState[ValidInput::EESC]) {
			if (over)
			{
				break;
			}
			double timeFrame = (double)(endT - startT) / CLOCKS_PER_SEC;

			if (timeFrame >= gTimeLog) {
				update(gKeyState);
				startT = clock();
			}

			keyUpdate(gKeyState);
			endT = clock();

			if (gamewin)
			{
				break;
			}
		}
		system("cls");

		if (gKeyState[ValidInput::EESC] || over)
		{
			std::cout << "Game Over" << std::endl;
			break;
		}
	}

	if (gamewin)
	{
		std::cout << "You Win!" << std::endl;
	}

	system("pause");
	return 0;
}

void keyUpdate(bool key[])
{
	for (int i = 0; i < ValidInput::INVALID; i++) {
		key[i] = false;
	}
	char input = '.';

	const int TIMEOUT = 1;
	clock_t tstart = clock();
	while ((clock() - tstart) / CLOCKS_PER_SEC < TIMEOUT) {
		if (_kbhit()) {
			input = _getch();
			break;
		}
	}
	Creature::respawn(gCreature);
	switch (input) {
	case 'w':
		key[ValidInput::EW] = true;
		break;
	case 's':
		key[ValidInput::ES] = true;
		break;
	case 'a':
		key[ValidInput::EA] = true;
		break;
	case 'd':
		key[ValidInput::ED] = true;
		break;
	case ' ':
		key[ValidInput::ESPACE] = true;
		break;
	case 27:
		key[ValidInput::EESC] = true;
		break;
	case '.':
		// do nothing
		break;
	default:
		break;
	}
}

bool isPositionValid(Position& pos)
{
	if (gBoard[pos.y][pos.x] == GNOTHING) {
		return true;
	}
	return false;
}

float clip(float n, float minimum, float maximum)
{
	return std::max(minimum, std::min(n, maximum));
}

bool canSee(Position cPos, Position hPos, Position& dir, int range)
{
	//// the dir_x and dir_y value are call-by-refernce
	dir.x = (int)clip((float)(hPos.x - cPos.x), -1.f, 1.f); // clip the value between -1 ~ 1
	dir.y = (int)clip((float)(hPos.y - cPos.y), -1.f, 1.f);


	// See Up
	for (int i = 1; i <= range; i++)
	{
		if (gBoard[cPos.y - i][cPos.x] == GWALL)
		{
			break;
		}
		if (cPos.x == hPos.x && cPos.y - i == hPos.y)
		{
			return true;
		}
	}

	// See Down
	for (int i = 1; i <= range; i++)
	{
		if (gBoard[cPos.y + i][cPos.x] == GWALL)
		{
			break;
		}
		if (cPos.x == hPos.x && cPos.y + i == hPos.y)
		{
			return true;
		}
	}

	// See Left
	for (int i = 1; i <= range; i++)
	{
		if (gBoard[cPos.y][cPos.x - i] == GWALL)
		{
			break;
		}
		if (cPos.x - i == hPos.x && cPos.y == hPos.y)
		{
			return true;
		}
	}

	// See Right
	for (int i = 1; i <= range; i++)
	{
		if (gBoard[cPos.y][cPos.x + i] == GWALL)
		{
			break;
		}
		if (cPos.x == hPos.x + i && cPos.y== hPos.y)
		{
			return true;
		}
	}

	if (cPos.y == hPos.y && cPos.x == hPos.x)
	{
		return true;
	}

	return false;
}

void setupBoard()
{
	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			if (i == 0 || i == GHEIGHT - 1) {
				gBoard[i][j] = GWALL;
			}
			else if (j == 0 || j == GWIDTH - 1) {
				gBoard[i][j] = GWALL;
			}
			else {
				gBoard[i][j] = GNOTHING;
			}
		}
	}
}

void fillBoard()
{
	gBoard = new std::string * [GHEIGHT];
	for (int i = 0; i < GHEIGHT; i++) {
		gBoard[i] = new std::string[GWIDTH];
		for (int j = 0; j < GWIDTH; j++) {
			gBoard[i][j] = GWALL;
		}
	}

	mazeWalk(gBoard);
}

void mazeWalk(std::string** board)
{
	// Implement Determination of start position and make start of recursion
	/*Please implement your code here*/
	setupBoard();
	srand(time(NULL));
	int width = GWIDTH - 2;
	int height = GHEIGHT - 2;
	Position startingPosition{ 0, 0 };
	mazeStep(board, startingPosition, width, height);
	/************************************************************************/
}

void mazeStep(std::string** board, Position pos, int width, int height)
{

	//base case is when one of the width or height is equal to one, since if it is the case, drawing line will be meaningless
	if (width < 3 || height < 3)
	{
		return;
	}

	int direction; //0 for horizontal, 1 for vertical
	if (width < height)
	{
		direction = 0;
	}
	else if (width > height)
	{
		direction = 1;
	}
	else
	{
		direction = rand() % 2; //same width and height (not 1), so it is random
	}
	//start setting the limit
	Position limitPosition;
	Position startPosition;
	Position gatePosition;
	if (direction == 0)
	{
		//horizontal direction
		//the x limit is width + 1
		limitPosition.x = pos.x + width + 1;
		//for the y, it will be randomized
		int randomSelection = rand() % (height / 2); //keep in mind that this needs to be divided by 2 because we need to make sure wall is odd indexed

		limitPosition.y = pos.y + 2 + (2 * randomSelection); //notice that all is multiply by 2, to make sure wall is only odd number

		//now that all is set, try to also set the start position
		startPosition.x = pos.x;
		startPosition.y = limitPosition.y;

		//find the gate Position
		randomSelection = rand() % (width / 2);
		gatePosition.x = pos.x + 1 + (2 * randomSelection); //notice it is only added by one because we want the gate to be even number
		gatePosition.y = limitPosition.y;

	}
	else
	{
		//vertical direction
		//the y limit is height + 1
		limitPosition.y = pos.y + height + 1;
		//for the x, it will be randomized
		int randomSelection = rand() % (width / 2); //keep in mind that this needs to be divided by 2 because we need to make sure wall is odd indexed

		limitPosition.x = pos.x + 2 + (2 * randomSelection); //notice that all is multiply by 2, to make sure wall is only odd number

		//now that all is set, try to also set the start position
		startPosition.x = limitPosition.x;
		startPosition.y = pos.y;

		//find the gate Position
		randomSelection = rand() % (height / 2);
		gatePosition.x = limitPosition.x; //notice it is only added by one because we want the gate to be even number
		gatePosition.y = pos.y + 1 + (2 * randomSelection);
	}

	//starting drawing line
	Position roamingPosition = startPosition;
	while (roamingPosition.x != limitPosition.x || roamingPosition.y != limitPosition.y)
	{
		if (roamingPosition.x != gatePosition.x || roamingPosition.y != gatePosition.y)
		{
			board[roamingPosition.y][roamingPosition.x] = GWALL;
		}
		else
		{
			//a gate, leave it blank
			board[roamingPosition.y][roamingPosition.x] = GNOTHING;
		}

		if (roamingPosition.x != limitPosition.x)
		{
			roamingPosition.x++;
		}
		if (roamingPosition.y != limitPosition.y)
		{
			roamingPosition.y++;
		}
	}

	//set for the next recursion, we set it to two region, where one of the region start with pos, and the other start with startPosition
	int newWidth1, newHeight1, newWidth2, newHeight2;
	if (direction == 0)
	{
		//if horizontal, the width is the same
		newWidth1 = width;
		newWidth2 = width;
		newHeight1 = startPosition.y - pos.y - 1;
		newHeight2 = height - newHeight1 - 1;
	}
	else
	{
		//if vertical, the height is the same
		newWidth1 = startPosition.x - pos.x - 1;
		newWidth2 = width - newWidth1 - 1;
		newHeight1 = height;
		newHeight2 = height;

	}

	//recursive call
	mazeStep(board, pos, newWidth1, newHeight1);
	mazeStep(board, startPosition, newWidth2, newHeight2);
}


void draw()
{
	// Add the hero into the board
	std::vector<std::vector<std::string>> drawBoard(GHEIGHT, std::vector<std::string>(GWIDTH));
	// std::string drawBoard[GHEIGHT][GWIDTH];

	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			drawBoard[i][j] = gBoard[i][j];
		}
	}

	// Update the hero information
	Position h = gHero.getPos();
	drawBoard[h.y][h.x] = gHero.getIcon();

	for (int i = 0; i < gCreature.size(); i++)
	{
		Position tmp = gCreature[i]->getPos();
		drawBoard[tmp.y][tmp.x] = gCreature[i]->getIcon();
	}

	for (int i = 0; i < gItem.size(); i++)
	{
		Position tmp = gItem[i]->getPos();
		drawBoard[tmp.y][tmp.x] = gItem[i]->getIcon();
	}

	if (starObtain == 3)
	{
		drawBoard[GHEIGHT - 2][GWIDTH - 2] = "F";
	}

	for (int i = 0; i < gCreature.size(); i++)
	{
		Position tmp = gCreature[i]->getPos();
		if (h.x == tmp.x && h.y == tmp.y)
		{
			gHero.damage(gCreature[i]->getAttackHit());
		}
	}


	/************************************************************************/


	// Draw the board
	std::cout << "Collect  ";
	SetConsoleTextAttribute(hConsole, 14);
	std::cout << starObtain;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "/3 star to open the finish gate" << std::endl;
	for (int i = 0; i < GHEIGHT; i++) {
		for (int j = 0; j < GWIDTH; j++) {
			if (drawBoard[i][j] == "H")
			{
				SetConsoleTextAttribute(hConsole, 11);
			}
			else if (drawBoard[i][j] == "G" || drawBoard[i][j] == "C" || drawBoard[i][j] == "K")
			{
				SetConsoleTextAttribute(hConsole, 13);
			}
			else if (drawBoard[i][j] == "!")
			{
				SetConsoleTextAttribute(hConsole, 12);
			}
			else if (drawBoard[i][j] == "T")
			{
				SetConsoleTextAttribute(hConsole, 10);
			}
			else if (drawBoard[i][j] == "/")
			{
				SetConsoleTextAttribute(hConsole, 10);
			}
			else if (drawBoard[i][j] == "*" || drawBoard[i][j] == "F")
			{
				SetConsoleTextAttribute(hConsole, 14);
			}
			else if (drawBoard[i][j] == "o")
			{
				SetConsoleTextAttribute(hConsole, 10);
			}
			else if (drawBoard[i][j] == GWALL)
			{
				if (stage == 1)
				{
					SetConsoleTextAttribute(hConsole, 38);
				}
				if (stage == 2)
				{
					SetConsoleTextAttribute(hConsole, 187);
				}
				if (stage == 3)
				{
					SetConsoleTextAttribute(hConsole, 68);
				}
				
			}
			std::cout << drawBoard[i][j]; //  output
			SetConsoleTextAttribute(hConsole, 7);
		}
		std::cout << "\n";
	}

	int heroHealth = gHero.getHealth();
	if (heroHealth <= 0)
	{
		over = true;
	}

	if (h.x == winPos.x && h.y == winPos.y && starObtain == 3)
	{
		gamewin = true;
	}

}

void drawInfo(void)
{
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "The hero is level ";
	SetConsoleTextAttribute(hConsole, 11);
	std::cout << gHero.getLevel() << "\n";
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "Reach level 5 more to perform attack on the enemy and knock them back!" << std::endl;
	std::cout << "when you step on the Item You can receive 15 Exp" << std::endl;
	std::cout << "has ";
	SetConsoleTextAttribute(hConsole, 10);
	std::cout << gHero.getExp();
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "/" << gHero.getMaxExp() << " EXP" << "\n";
	std::cout << "Hero health: ";
	SetConsoleTextAttribute(hConsole, 13);
	std::cout << gHero.getHealth() << std::endl;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "Hero attack: ";
	SetConsoleTextAttribute(hConsole, 12);
	std::cout << gHero.getAttack() << std::endl;
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "Use arrow key to move Hero @\n";
	std::cout << "Press ESC key to exit\n";

}

void update(bool key[])
{
	// �M������
	system("CLS");

	// �O�_��input
	bool hasInput = false;
	bool skillPress = false;
	int heroLevel = 0;
	try
	{
		if (key[ValidInput::EW]) {
			gHero.move(0, -1);
			hasInput = true;
		}
		else if (key[ValidInput::ES]) {
			gHero.move(0, 1);
			hasInput = true;
		}
		else if (key[ValidInput::EA]) {
			gHero.move(-1, 0);
			hasInput = true;
		}
		else if (key[ValidInput::ED]) {
			gHero.move(1, 0);
			hasInput = true;
		}
		else if (key[ValidInput::ESPACE])
		{
			int level = gHero.getLevel();
			if (level >= 5)
			{
				for (auto iterator = gCreature.begin(); iterator != gCreature.end(); iterator++)
				{
					//check all area up down left right
					if ((*iterator)->getPos().x == gHero.getPos().x - 1 && (*iterator)->getPos().y == gHero.getPos().y)
					{
						gHero.setHealth(gHero.getHealth() - (*iterator)->getAttackHit());
					}
					else if ((*iterator)->getPos().x == gHero.getPos().x + 1 && (*iterator)->getPos().y == gHero.getPos().y)
					{
						gHero.setHealth(gHero.getHealth() - (*iterator)->getAttackHit());
					}
					else if ((*iterator)->getPos().x == gHero.getPos().x && (*iterator)->getPos().y == gHero.getPos().y - 1)
					{
						gHero.setHealth(gHero.getHealth() - (*iterator)->getAttackHit());
					}
					else if ((*iterator)->getPos().x == gHero.getPos().x && (*iterator)->getPos().y == gHero.getPos().y + 1)
					{
						gHero.setHealth(gHero.getHealth() - (*iterator)->getAttackHit());
					}
				}
				skillPress = true;
				heroLevel = level;
			}
			hasInput = true;
		}
		else {
			throw 101;
		}
	}
	catch (...)
	{
		std::cout << "invalid input\n";
	}

	for (int i = 0; i < gCreature.size(); i++)
	{
		gCreature[i]->update(gHero.getPos(), skillPress, heroLevel, gHero.getAttack());
	}
	for (int i = 0; i < gItem.size(); i++)
	{
		gItem[i]->update(gHero, stage, starObtain, gItem);
	}
	draw();
	drawInfo();
}
