// Player class

#include "stdafx.h"
#include "Player.h"
#include "TextureHolder.h"

Player::Player() // constructor
{
	m_speed = START_SPEED;
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;

	m_sprite =sf::Sprite(TextureHolder::GetTexture("graphics/player.png"));
	// set center as origin
	m_sprite.setOrigin(25, 25);
}
void Player::Spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize)
{
	// ubicar el player en el centro de la arena
	m_position.x = arena.width / 2;
	m_position.y = arena.height / 2;
	// copiar datos de la arena al player
	m_arena.left = arena.left;
	m_arena.width = arena.width;
	m_arena.top = arena.top;
	m_arena.height = arena.height;

	// tamaño de los tiles de la arena
	m_tileSize = tileSize;
	// Guardar resolucion para uso futuro
	m_resolution.x = resolution.x;
	m_resolution.y = resolution.y;
}

void Player::PlayerStatReset()
{
	m_speed = START_SPEED;
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;
}

bool Player::Hit(sf::Time timeHit)
{
	if (timeHit.asMilliseconds()-m_lastHit.asMilliseconds()>200)
	{
		m_lastHit = timeHit;
		m_health -= 10; // TODO cambiar numero magico.
		return true;
	}
	else
	{
		return false;
	}
}

sf::Time Player::GetLastHit()
{
	return m_lastHit;
}

sf::FloatRect Player::GetPosition()
{
	return m_sprite.getGlobalBounds();
}

sf::Vector2f Player::GetCenter()
{
	return m_position;
}

float Player::GetRotation()
{
	return m_sprite.getRotation();
}

sf::Sprite Player::GetSprite()
{
	return m_sprite;
}

void Player::MoveUp()
{
	m_upPressed = true;
}

void Player::MoveDown()
{
	m_downPressed = true;
}

void Player::MoveRight()
{
	m_rightPressed = true;
}

void Player::MoveLeft()
{
	m_leftPressed = true;
}

void Player::StopMoveUp()
{
	m_upPressed = false;
}

void Player::StopMoveDown()
{
	m_downPressed = false;
}

void Player::StopMoveRight()
{
	m_rightPressed = false;
}

void Player::StopMoveLeft()
{
	m_leftPressed = false;
}

void Player::Update(float elapseTime, sf::Vector2i mousePosition)
{
	if (m_upPressed) { m_position.y -= m_speed * elapseTime; }
	if (m_downPressed) { m_position.y += m_speed * elapseTime; }
	if (m_rightPressed) { m_position.x += m_speed * elapseTime; }
	if (m_leftPressed) { m_position.x -= m_speed * elapseTime; }
	// position the player.
	m_sprite.setPosition(m_position);
	// keep player inside arena
	if(m_position.x > m_arena.width - m_tileSize) 
	{
		m_position.x = m_arena.width - m_tileSize;
	}
	if (m_position.x <m_arena.left + m_tileSize) 
	{
		m_position.x = m_arena.left + m_tileSize;
	}
	if (m_position.y > m_arena.height-m_tileSize)
	{
		m_position.y = m_arena.height - m_tileSize;
	}
	if (m_position.y < m_arena.top + m_tileSize)
	{
		m_position.y = m_arena.top + m_tileSize;
	}
	// calculate rotation 
	float angle = atan2(mousePosition.y - m_resolution.y / 2, mousePosition.x - m_resolution.x / 2) * 180 / 3.141;
	// player rotation
	m_sprite.setRotation(angle);
}

void Player::UpgradeHealth()
{
	m_maxHealth += (START_HEALTH*.2);
}

void Player::UpgradeSpeed()
{
	m_speed += (START_SPEED*.2);
}

void Player::GetMaxHealth(int amount)
{
	m_health += amount;
	if (m_health > m_maxHealth )
	{
		m_health = m_maxHealth;
	}
}

int Player::GetHealth()
{
	return m_health;
}
