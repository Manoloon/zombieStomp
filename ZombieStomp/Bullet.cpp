#include "stdafx.h"
#include "Bullet.h"

// the constructor!!
Bullet::Bullet()
{
	m_BulletShape.setSize(sf::Vector2f(2, 2));
}

void Bullet::StopBullet()
{
	m_inFly = false;
}

bool Bullet::IsInFly()
{
	return m_inFly;
}

void Bullet::Shoot(float StartX, float StartY, float TargetX, float TargetY)
{
	m_inFly = true;
	m_Position.x = StartX;
	m_Position.y = StartY;

	// calculate gradient
	float gradient = (StartX - TargetX) / (StartY - TargetY);
	// any gradient less than 1 needs to be negative 
	if(gradient<0)
	{
		gradient *= -1;
	}
	// calculate ratio between x and y
	float ratioXY = m_BulletSpeed / (1 + gradient);
	// set speed 
	m_BulletDistanceX = ratioXY * gradient;
	m_BulletDistanceY = ratioXY;
	// point bullet in the right direction
	if(TargetX<StartX)
	{
		m_BulletDistanceX *= -1;
	}
	if (TargetY<StartY)
	{
		m_BulletDistanceY *= -1;

		// set max rangeof 1000 pixels
		float Range = 1000;
		m_MinX = StartX - Range;
		m_MaxX = StartX + Range;
		m_MinY = StartY - Range;
		m_MaxY = StartY + Range;

		// position the bullet to drawn
		m_BulletShape.setPosition(m_Position);
	}
}

sf::FloatRect Bullet::GetPosition()
{
	return m_BulletShape.getGlobalBounds();
}

sf::RectangleShape Bullet::GetShape()
{
	return m_BulletShape;
}

void Bullet::Update(float elapsedTime)
{
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	m_BulletShape.setPosition(m_Position);

	if(m_Position.x < m_MinX || m_Position.x > m_MaxX ||m_Position.y < m_MinY ||m_Position.y > m_MaxY)
	{
		m_inFly = false;
	}
}
