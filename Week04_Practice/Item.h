#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Position.h"
#include "Hero.h"


class Item {
public:
	Position sPos; // Hero location
	char sIcon;	// Hero icon
	int sExpAmount; //amount of exp
	int sHeal; //amount of heal
	int sAttack; //amount of attack boost

	// Default constructor
	Item(void) {
		this->sPos.x = 6;
		this->sPos.y = 7;
		this->sExpAmount = 5;
		this->sHeal = false;
		this->sAttack = 0;
		this->sIcon = 'T';
	};

	// Setting constructor
	Item(int x, int y, int amt, char icon = 'T') {
		this->sPos.x = x;
		this->sPos.y = y;
		this->sExpAmount = amt;
		this->sHeal = false;
		this->sAttack = 0;
		this->sIcon = icon;
	};

	// Setting constructor
	Item(Position& pos, int amt, char icon = 'T') {
		this->sPos = pos;
		this->sExpAmount = amt;
		this->sIcon = icon;
	};

	// Set position
	void setPos(Position pos) { this->sPos = pos; }
	void setPos(int x, int y) {
		this->sPos.x = x;
		this->sPos.y = y;
	}

	// Set icon
	void setIcon(char& icon) { this->sIcon = icon; }

	// Set the amount
	void SetAmount(int amt) { this->sExpAmount = amt; }

	// Get position
	Position getPos(void) { return this->sPos; }

	// Get Icon
	char getIcon(void) { return this->sIcon; }

	// Get the amount 
	int getExpAmount(void) { return this->sExpAmount; }


	//do action based on the type of the item (will be overwritten in the function of the derived class)
	virtual void item(Hero& h)
	{

	}

	void update(Hero& h, int stage, int &startot, std::vector<Item*> &gItem) {
		if (this->sPos.x == h.getPos().x && this->sPos.y == h.getPos().y) {
			this->item(h);
			if (this->sIcon == '*')
			{
				startot++;
				if (startot == 3)
				{
					gItem.pop_back();
				}
			}
			Position tmp;
			if (stage == 1)
			{
				tmp.x = 2 * (rand() % 3) + 1;
				tmp.y = 2 * (rand() % 3) + 1;
			}
			else if (stage == 2)
			{
				tmp.x = 2 * (rand() % 8) + 1;
				tmp.y = 2 * (rand() % 6) + 1;
			}
			else if (stage == 3)
			{
				tmp.x = 2 * (rand() % 19) + 1;
				tmp.y = 2 * (rand() % 8) + 1;
			}
			this->sPos.x = tmp.x;
			this->sPos.y = tmp.y;
		}
	}
};

class EXPTrigger : public Item
{
public:
	void item(Hero& h) 
	{
		h.gainEXP(sExpAmount);
	}

	EXPTrigger(void) {
		this->sPos.x = 6;
		this->sPos.y = 7;
		this->sExpAmount = 5;
		this->sHeal = false;
		this->sAttack = 0;
		this->sIcon = 'T';
	};

	// Setting constructor
	EXPTrigger(int x, int y, int amt, char icon = 'T') {
		this->sPos.x = x;
		this->sPos.y = y;
		this->sExpAmount = amt;
		this->sHeal = false;
		this->sAttack = 0;
		this->sIcon = icon;
	};

	// Setting constructor
	EXPTrigger(Position& pos, int amt, char icon = 'T') {
		this->sPos = pos;
		this->sExpAmount = amt;
		this->sIcon = icon;
	};
};

//Potion is used to store the hero's health
class Potion : public Item
{
public:
	void item(Hero& h) {
		if (this->sHeal)
		{
			int randNum = rand() % 3;
			if (randNum == 0) // reset health with a probability of 1/3
			{
				h.resetHealth();
			}
			else
			{
				int currentHealth = h.getHealth();
				if (currentHealth + 5 >= 40)
				{
					h.resetHealth();
				}
				else
				{
					h.setHealth(currentHealth + 5);
				}
			}
		}
	}

	Potion(int x, int y)
	{
		this->sPos.x = x;
		this->sPos.y = y;
		this->sExpAmount = 0;
		this->sAttack = 0;
		this->sHeal = true;
		this->sAttack = false;
		this->sIcon = 'o';
	}
};

//star is used to make sure that the hero need to collect three star before the exit opens
class Star : public Item
{
public:
	void item(Hero& h) 
	{
		// no need to do anything to the hero
	}

	Star(int x, int y)
	{
		this->sPos.x = x;
		this->sPos.y = y;
		this->sExpAmount = 0;
		this->sAttack = 0;
		this->sHeal = false;
		this->sAttack = false;
		this->sIcon = '*';
	}
};

//sword can be used to increase the attack of the hero by 10 to kill the creature
class Sword : public Item
{
public:
	void item(Hero& h)
	{
		//increase the attack of the hero
		h.setAttack(h.getAttack() + this->sAttack);
	}

	Sword(int x, int y, int attackBoost)
	{
		this->sPos.x = x;
		this->sPos.y = y;
		this->sExpAmount = 0;
		this->sAttack = attackBoost;
		this->sHeal = false;
		this->sIcon = '/';
	}
};