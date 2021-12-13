#pragma once
#include <string>
#include "Position.h"
#include "main.h"

class Hero {

private:
	Position	sPos;			// Hero location
	char sIcon = 'H';			// Hero icon
	int			sLevel = 1;     // Level
	int			sMaxExp;		// Level update experience
	int			sCurrentExp;	// current exp hero has
	int			health = 50;	// health of hero
	int			attack = 1;		// attack of the hero for killing the enemy creature

public:
	// Default constructor
	Hero() {
		this->sPos.x = 0;
		this->sPos.y = 0;
		this->sLevel = 1;
		this->sCurrentExp = 0;
		this->sMaxExp = 10;
		this->sIcon = 'H';
		this->attack = 1;
	};
	// Setting constructor
	Hero(int x, int y, int level = 1, int maxExp = 10, int currentExp = 0, char icon = 'H') {
		this->sPos.x = x;
		this->sPos.y = y;
		this->sLevel = level;
		this->sCurrentExp = currentExp;
		this->sMaxExp = maxExp;
		this->sIcon = icon;
		this->attack = 1;
	};

	// Setting constructor
	Hero(Position& pos, int level = 1, int maxExp = 10, int currentExp = 0, char icon = 'H') {
		this->sPos = pos;
		this->sLevel = level;
		this->sCurrentExp = currentExp;
		this->sMaxExp = maxExp;
		this->sIcon = icon;
		this->attack = 1;
	};

	// Set position
	void setPos(Position pos) { this->sPos = pos; }
	void setPos(int x, int y) {
		this->sPos.x = x;
		this->sPos.y = y;
	}

	// Set icon
	void setIcon(char& icon) { this->sIcon = icon; }

	// Set level
	void setLevel(int level) { this->sLevel = level; }

	// Set exp
	void setExp(int exp) { this->sCurrentExp = exp; }

	// Set MaxExp
	void SetMaxExp(int maxExp) { this->sMaxExp = maxExp; }

	//
	void setAttack(int attack)
	{
		this->attack = attack;
	}

	// Get position
	Position getPos(void) { return this->sPos; }

	// Get Icon
	char getIcon(void) { return this->sIcon; }

	// Get level
	int getLevel(void) { return this->sLevel; }

	// Get Exp
	int getExp(void) { return this->sCurrentExp; }

	// Get Max
	int getMaxExp(void) { return this->sMaxExp; }

	// Get Health
	int getHealth(void) { return this->health; }

	int getAttack(void)
	{
		return this->attack;
	}

	void setHealth(int hp)
	{
		this->health = hp;
	}

	void resetHealth()
	{
		this->health = 50;
	}

	// Incrementally move the elements 
	void move(int x, int y) {
		// Compute the next position
		Position next;
		next.x = this->sPos.x + x;
		next.y = this->sPos.y + y;

		try 
		{
			if (isPositionValid(next))
				this->sPos = next;
			else {
				throw 101;
			}
		}
		catch (...)
		{
			std::cout << "Invalid Location" << std::endl;
		}
	}

	// Calculate Gain of EXP
	void gainEXP(int points) {
		int number = points;
		while (number > 0) {
			if (sCurrentExp + number >= sMaxExp) {//level up
				number -= (sMaxExp - sCurrentExp);
				sCurrentExp = 0;
				sMaxExp = (int)((float)sMaxExp * 1.2f);
				sLevel++;
				attack++;
			}
			else {
				sCurrentExp += number;
				number = 0;
			}
		}
	}

	void damage(int points) {
		this->health -= points;
	}
};
