#pragma once
#include "stdafx.h"
#include "ZombieStomp.h"
#include "Zombie.h"

Zombie* CreateHorde(int NumZombies, sf::IntRect arena) 
{
	Zombie* zombies = new Zombie[NumZombies];

	int MaxY = arena.height - 20;
	int MinY = arena.top + 20;
	int MaxX = arena.width - 20;
	int MinX = arena.left + 20;

	for (int i = 0; i < NumZombies; i++) 
	{
		// select side to spawn zombies
		srand((int)time(0)*i);
		int side = (rand() % 4);

		float x, y;
		switch (side)
		{
		case 0:
			// left
			x = MinX;
			y = (rand() % MaxY) + MinY;
			break;
		case 1:
			//Right
			x = MaxX;
			y = (rand() % MaxY) + MinY;
			break;
		case 2:
			// Top
			x = (rand() % MaxX) + MinX;
			y = MinY;
			break;
		case 3:
			// Bottom
			x = (rand() % MaxX) + MinX;
			y = MaxY;
			break;
		}
		// choose type of zombie
		srand((int)time(0)*i * 2);
		int type = (rand() % 3);
		
		// spawn the new zombie into ARRay
		zombies[i].Spawn(x, y, type, i);
	}
	return zombies;
}