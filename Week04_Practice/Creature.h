#pragma once
#include <string>
#include "Position.h"
#include "main.h"
#include <vector>

class Creature {//creature class 
private:
	
public:
	Position sPos;			// Creature position
	char sIcon;			// Creature icon
	int attackHit;
	char status;
	int remainingHealth; //when reach 0, creature can also die
	int respawnCount = 0;

	//this will used to handle when the creature is death
	void static respawn(std::vector<Creature*> gCreature)
	{
		for (auto iterator = gCreature.begin(); iterator != gCreature.end(); iterator++)
		{
			//restore the health bar first, then immobilize the creature for 5 round
			if ((*iterator)->remainingHealth < 0)
			{
				if ((*iterator)->status == 'G')
				{
					(*iterator)->remainingHealth = 10;
					(*iterator)->sIcon = 'x';
					(*iterator)->respawnCount = 5;
				}
				else if ((*iterator)->status == 'C')
				{
					(*iterator)->remainingHealth = 20;
					(*iterator)->sIcon = 'x';
					(*iterator)->respawnCount = 5;
				}
				else if ((*iterator)->status == 'K')
				{
					(*iterator)->remainingHealth = 50;
					(*iterator)->sIcon = 'x';
					(*iterator)->respawnCount = 5;
				}
				else
				{
					std::cout << "Undefined status!\n";
				}
			}
			else if ((*iterator)->sIcon = 'x')
			{
				(*iterator)->respawnCount--;
				//restart to not being able mobilize again
				if ((*iterator)->respawnCount < 0)
				{
					(*iterator)->sIcon = (*iterator)->status;
				}
			}
		}
	}

	// Default constructor
	Creature(void) {
		this->sPos.x = 1;
		this->sPos.y = 1;
		this->sIcon = 'C';
		this->attackHit = 1;
		this->status = 'C';
		this->remainingHealth = 10;
	};

	// Set position
	void setPos(Position pos) { this->sPos = pos; }
	void setPos(int x, int y) {
		this->sPos.x = x;
		this->sPos.y = y;
	}

	// Set icon
	void setIcon(char& icon) { this->sIcon = icon; }

	// Get position
	Position getPos(void) { return this->sPos; }

	// Get Icon
	char getIcon(void) { return this->sIcon; }

	int getAttackHit(void) 
	{ 
		if (this->getIcon() == 'x')
		{
			return 0;
		}
		else
		{
			return this->attackHit;
		} 
	}

	//subtract remaining health
	void setRemainingHealth(int remainingHealth)
	{
		this->remainingHealth = remainingHealth;
	}
	//get remainingHealth
	int getRemainingHealth()
	{
		return this->remainingHealth;
	}

	void update(Position hPos, bool skillActivated, int heroLevel, int heroAttack) {
		Position dir;
		int searchRange = 0;
		bool inRange = false;
		if (this->status == 'G')
		{
			searchRange = 3;
		}
		else if (this->status == 'C')
		{
			searchRange = 15;
		}
		else if (this->status == 'K')
		{
			searchRange = 2;
		}

		bool pushed = false;
		// check hero surroundings
		if (skillActivated)
		{
			if (hPos.x == this->sPos.x && hPos.y - 1 == this->sPos.y)
			{
				Position test = sPos;
				if (heroLevel >= 10)
				{
					test.y--;
					while (isPositionValid(test))
					{
						test.y--;
					}
					test.y++;
				}
				else if (heroLevel >= 5)
				{
					test.y--;
					if (!isPositionValid(test))
					{
						test.y++;
					}
				}

				this->sPos.x = test.x;
				this->sPos.y = test.y;
				this->setRemainingHealth(getRemainingHealth() - heroAttack);
				pushed = true;
			}
			if (hPos.x == this->sPos.x && hPos.y + 1 == this->sPos.y)
			{
				Position test = sPos;
				if (heroLevel >= 10)
				{
					test.y++;
					while (isPositionValid(test))
					{
						test.y++;
					}
					test.y--;
				}
				else if (heroLevel >= 5)
				{
					test.y++;
					if (!isPositionValid(test))
					{
						test.y--;
					}
				}

				this->sPos.x = test.x;
				this->sPos.y = test.y;
				this->setRemainingHealth(getRemainingHealth() - heroAttack);
				pushed = true;
			}
			if (hPos.x - 1 == this->sPos.x && hPos.y == this->sPos.y)
			{
				Position test = sPos;
				if (heroLevel >= 10)
				{
					test.x--;
					while (isPositionValid(test))
					{
						test.x--;
					}
					test.x++;
				}
				else if (heroLevel >= 5)
				{
					test.x--;
					if (!isPositionValid(test))
					{
						test.x++;
					}
				}

				this->sPos.x = test.x;
				this->sPos.y = test.y;
				this->setRemainingHealth(getRemainingHealth() - heroAttack);
				pushed = true;
			}
			if (hPos.x + 1 == this->sPos.x && hPos.y == this->sPos.y)
			{
				Position test = sPos;
				if (heroLevel >= 10)
				{
					test.x++;
					while (isPositionValid(test))
					{
						test.x++;
					}
					test.x--;
				}
				else if (heroLevel >= 5)
				{
					test.x++;
					if (!isPositionValid(test))
					{
						test.x--;
					}
				}

				this->sPos.x = test.x;
				this->sPos.y = test.y;
				this->setRemainingHealth(getRemainingHealth() - heroAttack);
				pushed = true;
			}
		}

		if (!pushed && this->getIcon() != 'x')
		{
			if (canSee(this->sPos, hPos, dir, searchRange)) {
				// std::cout << "Hero is in range in the ( " << dir.x << ", " << dir.y << " ) direction\n";
				this->sIcon = '!';
				int ratio = 0;
				if (this->status == 'G')
				{
					ratio = 4;
				}
				else if (this->status == 'C')
				{
					if (this->sPos.x == hPos.x && this->sPos.y == hPos.y)
					{
						ratio = 4;
					}
					else
					{
						ratio = 1;
					}
				}
				else if (this->status == 'K')
				{
					ratio = 8;
				}

				int randNum = rand() % ratio;
				if (randNum == 0)
				{
					Position test = sPos;
					test.x += dir.x;
					test.y += dir.y;
					if (isPositionValid(test))
					{
						this->sPos.x += dir.x;
						this->sPos.y += dir.y;
					}
				}
			}
			else
			{
				// std::cout << "Goblin didn't find the Hero\n";
				if (this->status == 'G')
				{
					this->sIcon = 'G';
				}
				else if (this->status == 'C')
				{
					this->sIcon = 'C';
				}
				else if (this->status == 'K')
				{
					this->sIcon = 'K';
				}
				bool found = false;
				do
				{
					int randNum = rand() % 4;
					if (randNum == 0)
					{
						Position test = sPos;
						test.x += 1;
						if (isPositionValid(test))
						{
							found = true;
							this->sPos.x += 1;
						}
					}
					if (randNum == 1)
					{
						Position test = sPos;
						test.x -= 1;
						if (isPositionValid(test))
						{
							found = true;
							this->sPos.x -= 1;
						}
					}
					if (randNum == 2)
					{
						Position test = sPos;
						test.y += 1;
						if (isPositionValid(test))
						{
							found = true;
							this->sPos.y += 1;
						}
					}
					if (randNum == 3)
					{
						Position test = sPos;
						test.y -= 1;
						if (isPositionValid(test))
						{
							found = true;
							this->sPos.y -= 1;
						}
					}
				} while (!found);
			}
		}
	}
};

class Goblin : public Creature
{
public:
	Goblin(int posX, int posY)
	{
		this->sPos.x = posX;
		this->sPos.y = posY;
		this->sIcon = 'G';
		this->status = 'G';
		this->attackHit = 3;
		this->remainingHealth = 10;
	}
};

class Chaser : public Creature
{
public:
	Chaser(int posX, int posY)
	{
		this->sPos.x = posX;
		this->sPos.y = posY;
		this->sIcon = 'C';
		this->status = 'C';
		this->attackHit = 1;
		this->remainingHealth = 20;
	}
};

class Killer : public Creature
{
public:
	Killer(int posX, int posY)
	{
		this->sPos.x = posX;
		this->sPos.y = posY;
		this->sIcon = 'K';
		this->status = 'K';
		this->attackHit = 8;
		this->remainingHealth = 50;
	}
};