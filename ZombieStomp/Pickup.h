#pragma once
#include <SFML\Graphics.hpp>

class Pickup 
{
private :
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	sf::Sprite m_Sprite;
	sf::IntRect m_Arena;
	// valor del item
	int m_Value;
	// tipo :1 Health - 0 Ammo
	int m_Type;
	// handle spawn and despawn
	bool m_Spawned;

	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:
	Pickup::Pickup(int Type);
	void SetArena(sf::IntRect arena);
	void Spawn();
	sf::FloatRect GetPosition();
	sf::Sprite GetSprite();
	void Update(float ElapsedTime);
	bool IsSpawned();
	int GotIt();
	void Upgrade();
};
