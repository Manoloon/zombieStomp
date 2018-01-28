#pragma once
#include <SFML/Graphics.hpp>

class Player 
{
private :
	const float START_SPEED = 200;
	const float START_HEALTH = 100;
	sf::Vector2f m_position;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Vector2f m_resolution;
	// tamaño de la arena
	sf::IntRect m_arena;
	// tamaño de los tiles
	int m_tileSize;
	int m_maxHealth;
	sf::Time m_lastHit;
	float m_speed;
	int m_health;
	// input
	bool m_upPressed;
	bool m_downPressed;
	bool m_leftPressed;
	bool m_rightPressed;

public:
	Player();
	void Spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);
	void PlayerStatReset();
	bool Hit(sf::Time timeHit);
	sf::Time GetLastHit();
	sf::FloatRect GetPosition();
	sf::Vector2f GetCenter();
	float GetRotation();
	sf::Sprite GetSprite();
	// input
	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();
	
	void StopMoveUp();
	void StopMoveDown();
	void StopMoveRight();
	void StopMoveLeft();
	////////////////////////
	void Update(float elapseTime,sf::Vector2i mousePosition);
	///////////////////////
	void UpgradeHealth();
	void UpgradeSpeed();
	void GetMaxHealth(int amount);
	int GetHealth();
};

