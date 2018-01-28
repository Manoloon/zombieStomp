#pragma once
#include "SFML\Graphics.hpp"

class Zombie 
{
private:
	// 3 types of zombies
	const float CRAWLER_SPEED = 20;
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;

	const float CRAWLER_HEALTH = 5;
	const float BLOATER_HEALTH = 3;
	const float CHASER_HEALTH = 1;

	// make each zombie vary speed
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	sf::Vector2f m_Position;
	sf::Sprite m_Sprite;
	float m_Speed;
	float m_Health;
	
	// check life
	bool m_IsAlive;

public:
	void Spawn(float StartX, float StartY, int Type, int Seed);
	sf::FloatRect GetPosition();
	bool Hit();
	bool IsAlive();
	void update(float ElapseTime,sf::Vector2f PlayerLocation);
	sf::Sprite GetSprite();
};
