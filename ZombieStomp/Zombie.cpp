#pragma once
#include "stdafx.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

void Zombie::Spawn(float StartX, float StartY, int Type, int Seed)
{
	switch (Type)
	{
		//bloater
	case 0:
		m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = 40;
		m_Health = 5;
		break;
		// Chaser
	case 1:
		m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = 80;
		m_Health = 1;
		break;
		// Crawler
	case 2:
		m_Sprite = sf::Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = 20;
		m_Health = 3;
		break;
	}
	// some random to the speed
	srand((int)time(0)*Seed);
	float Modifier = (rand() % MAX_VARIANCE) + OFFSET;
	Modifier /= 100; // betwee .7 and 1
	m_Speed *= Modifier;
	//location
	m_Position.x = StartX;
	m_Position.y = StartY;

	m_Sprite.setOrigin(25, 25); // TODO HARDCODE Numbers
	m_Sprite.setPosition(m_Position);
}

sf::FloatRect Zombie::GetPosition()
{
	return m_Sprite.getGlobalBounds();
}

bool Zombie::Hit()
{
	m_Health--;
	if (m_Health <= 0)
	{
		m_IsAlive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	else 
	{
		return false;
	}		
}

bool Zombie::IsAlive()
{
	return m_IsAlive;
}

void Zombie::update(float ElapseTime, sf::Vector2f PlayerLocation)
{
	float PlayerX = PlayerLocation.x;
	float PlayerY = PlayerLocation.y;

	// position on X contra el player.
	if (PlayerX > m_Position.x)
	{
		m_Position.x = m_Position.x + m_Speed * ElapseTime;
	}
	if (PlayerX < m_Position.x)
	{
		m_Position.x = m_Position.x - m_Speed * ElapseTime;
	}
	// position on Y contra el player.
	if (PlayerY > m_Position.y)
	{
		m_Position.y = m_Position.y + m_Speed * ElapseTime;
	}
	if (PlayerY < m_Position.y)
	{
		m_Position.y = m_Position.y - m_Speed * ElapseTime;
	}
	
	// move the sprite 
	m_Sprite.setPosition(m_Position);
	// angle looking to the player.
	float Angle = (atan2(PlayerY - m_Position.y, PlayerX - m_Position.x) * 180) / 3.141;
		m_Sprite.setRotation(Angle);
		
}

sf::Sprite Zombie::GetSprite()
{
	return m_Sprite;
}
