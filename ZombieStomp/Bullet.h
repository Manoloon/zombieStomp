#pragma once
#include <SFML/Graphics.hpp>

class Bullet 
{
private:
	sf::Vector2f m_Position;
	sf::RectangleShape m_BulletShape;
	bool m_inFly = false;
	float m_BulletSpeed = 1000;

	float m_BulletDistanceX;
	float m_BulletDistanceY;

	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:
	// constructor
	Bullet();

	void StopBullet();
	bool IsInFly();
	void Shoot(float StartX, float StartY, float TargetX, float TargetY);
	sf::FloatRect GetPosition();
	sf::RectangleShape GetShape();
	void Update(float ElapseTime);
};
