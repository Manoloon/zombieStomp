// Main file for the game

#pragma once	
#include "stdafx.h"
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ZombieStomp.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

int main()
{
	// instance of TEXTURE HOLDER
	TextureHolder Holder;
	// define 4 states of the game. 
	enum class EGameState
	{
		PAUSED,LEVELING_UP,GAME_OVER,PLAYING
	};
	EGameState state = EGameState::GAME_OVER;
	// define resolution
	sf::Vector2f resolution;
	resolution.x = sf::VideoMode::getDesktopMode().width;
	resolution.y = sf::VideoMode::getDesktopMode().height;
	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Stomp", sf::Style::Fullscreen);
	
	// SFML VIEW CREATE for MAIÇN Section-
	sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// define clock for timing !!
	sf::Clock clock;

	// how long the player was playing?
	sf::Time gameTimeTotal;

	sf::Vector2f mouseWorldPosition; // mouse en el mundo
	sf::Vector2i mouseScreenPosition; // mouse en coordenadas de pantalla.

	// instanciamos el player.
	Player player;
	// limites de la arena.
	sf::IntRect arena;
	//crea la arena con los tileset
	sf::VertexArray background;
	sf::Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");

	// prepare horde of zombies!
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	// 100 should do
	Bullet bullets[100];
	int CurrentBullet = 0;
	int BulletSpare = 24;
	int BulletInClip = 6;
	int ClipSize = 6;
	float FireRate = 1;
	sf::Time LastPressedFire;

	// crosshair
	window.setMouseCursorVisible(true);
	sf::Sprite sprCrosshair;
	sf::Texture texCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	sprCrosshair.setTexture(texCrosshair);
	sprCrosshair.setOrigin(25, 25);

	// initialize PICKUPS
	Pickup healthPickup(1);
	Pickup AmmoPickup(2);

	// Set the GAME!!!
	int Score = 0;
	int HiScore = 0;

	// forthe home/game over screen
	sf::Sprite sprGameOver;
	sf::Texture TexGameOver = TextureHolder::GetTexture("graphics/background.png");
	sprGameOver.setTexture(TexGameOver);
	sprGameOver.setPosition(0, 0);

	//create view for the HUD
	sf::View HudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	//sprite for ammo HUD
	sf::Sprite sprAmmoHud;
	sf::Texture TexAmmoHud = TextureHolder::GetTexture("graphics/ammo_icon.png");
	sprAmmoHud.setTexture(TexAmmoHud);
	sprAmmoHud.setPosition(20, 980);

	//TODO - put all this on a header

	// Load Font
	sf::Font HudFont;
	HudFont.loadFromFile("fonts/zombiecontrol.ttf");

	// Paused
	sf::Text PauseTxt;
	PauseTxt.setFont(HudFont);
	PauseTxt.setCharacterSize(125);
	PauseTxt.setFillColor(sf::Color::White);
	PauseTxt.setPosition(400, 400);
	PauseTxt.setString("Press Enter \n to Continue");

	// GameOver
	sf::Text GameOverTxt;
	GameOverTxt.setFont(HudFont);
	GameOverTxt.setCharacterSize(125);
	GameOverTxt.setFillColor(sf::Color::Red);
	GameOverTxt.setPosition(150, 250);
	GameOverTxt.setString("ZOMBIE STOMP!! \n PRESS ENTER TO PLAY");

	// Leveling up
	sf::Text LevelUpTxt;
	LevelUpTxt.setFont(HudFont);
	LevelUpTxt.setCharacterSize(80);
	LevelUpTxt.setFillColor(sf::Color::White);
	LevelUpTxt.setPosition(150, 250);
	std::stringstream LevelUpStream;
	LevelUpStream << "1-Increase rate of Fire" << "\n2-Increase Clip Size" << "\n3-Increase Max Health" << "\n4-Increase Run Speed" << "\n5-Upgrade Health Pickup" << "\n6-Upgrade Ammo Pickup ";
	LevelUpTxt.setString(LevelUpStream.str());

	//Ammo 
	sf::Text AmmoTxt;
	AmmoTxt.setFont(HudFont);
	AmmoTxt.setCharacterSize(55);
	AmmoTxt.setFillColor(sf::Color::White);
	AmmoTxt.setPosition(200, 980);

	//Score
	sf::Text ScoreTxt;
	ScoreTxt.setFont(HudFont);
	ScoreTxt.setCharacterSize(55);
	ScoreTxt.setFillColor(sf::Color::White);
	ScoreTxt.setPosition(20, 0);

	//load hiscore from text file
	std::ifstream InputFile("gamedata/hiscore.txt");
	if (InputFile.is_open())
	{
		InputFile >> HiScore;
		InputFile.close();
	}

	//Hiscore
	sf::Text HiscoreTxt;
	HiscoreTxt.setFont(HudFont);
	HiscoreTxt.setCharacterSize(55);
	HiscoreTxt.setFillColor(sf::Color::Yellow);
	HiscoreTxt.setPosition(1400, 0);
	std::stringstream s;
	s << "HI SCORE:" << HiScore;
	HiscoreTxt.setString(s.str());

	// Zombie Remaining
	sf::Text ZombiesRemainingTxt;
	ZombiesRemainingTxt.setFont(HudFont);
	ZombiesRemainingTxt.setCharacterSize(55);
	ZombiesRemainingTxt.setFillColor(sf::Color::White);
	ZombiesRemainingTxt.setPosition(1500, 980);
	ZombiesRemainingTxt.setString("Zombies: 100");

	// Wave Number
	int wave = 0;
	sf::Text WaveTxt;
	WaveTxt.setFont(HudFont);
	WaveTxt.setCharacterSize(55);
	WaveTxt.setFillColor(sf::Color::Yellow);
	WaveTxt.setPosition(1250, 980);
	WaveTxt.setString("Wave : 0");

	// Healthbar
	sf::RectangleShape healthBar;
	healthBar.setFillColor(sf::Color::Red);
	healthBar.setPosition(450, 980);

	// hasta aqui!
	//when did we last update the hud?
	int framesSinceLastHudUpdate = 0;
	//how often should we update the hud.
	int fpsMeasureFrameInterval = 1000;
	
	// main game loop!!! 
	while (window.isOpen())
	{
		/*
		maneja los inputs
		*/
		// maneja eventos por pedidos. 
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::KeyPressed)
			{
				// paused the game while playing
				if (event.key.code == sf::Keyboard::Return && state == EGameState::PLAYING)
				{
					state = EGameState::PAUSED; 
				} // TODO poder definir tecla de pausa

				//restart while paused
				else if (event.key.code == sf::Keyboard::Return && state == EGameState::PAUSED)
				{
					state = EGameState::PLAYING;
					// reset the clock
					clock.restart();
				}
				// START new game
				else if (event.key.code == sf::Keyboard::Return && state == EGameState::GAME_OVER) 
				{ 
					state = EGameState::LEVELING_UP; 
				}
				if (state == EGameState::PLAYING) // TODO no definido!!!! !
				{
				//Reloading 
					if (event.key.code==sf::Keyboard::R)
					{
						if(BulletSpare>=ClipSize)
						{
							BulletInClip = ClipSize;
							BulletSpare -= ClipSize;
						}
						else if (BulletSpare>0)
						{
							BulletInClip = BulletSpare;
							BulletSpare = 0;
						}
						else 
						{
						// TODO 
						}
					}
				} 
				}
			}// end EVENT POLLING
		
		 // QUIT THE GAME
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) 
		{ 
			window.close(); 
		}

		// WASD move
		if (state == EGameState::PLAYING) // TODO - make key selectable
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { player.MoveUp(); }
			else { player.StopMoveUp(); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) { player.MoveDown(); }
			else { player.StopMoveDown(); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { player.MoveRight(); }
			else { player.StopMoveRight(); }
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { player.MoveLeft(); }
			else { player.StopMoveLeft(); }
			// Shooting
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if ((gameTimeTotal.asMilliseconds() - LastPressedFire.asMilliseconds()) > 1000 / FireRate && (BulletInClip > 0))
				{
					//pass center of the player and the center of crosshair to shoot function
					bullets[CurrentBullet].Shoot(player.GetCenter().x, player.GetCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					CurrentBullet++;
					if (CurrentBullet > 99)
					{
						CurrentBullet = 0;
					}
					LastPressedFire = gameTimeTotal;
					BulletInClip--;
				}
			}//end of fire
		}//end of movement

	    // LEVELING
		if (state == EGameState::LEVELING_UP)
		{
			if (event.key.code == sf::Keyboard::Num1) { state = EGameState::PLAYING; }
			if (event.key.code == sf::Keyboard::Num2) { state = EGameState::PLAYING; } // TODO hacer hasta Num6 -Reemplazar por tecla.
			if (state == EGameState::PLAYING)
			{
				// prepare the level
				arena.width = 1000;
				arena.height = 1000;
				arena.left = 0;
				arena.top = 0;
				// pass vertex array by reference 
				int tileSize = CreateBackground(background, arena);

				//spawn the player
				player.Spawn(arena, resolution, tileSize);

				// Configure the pickups 
				healthPickup.SetArena(arena);
				AmmoPickup.SetArena(arena);

				// Zombie horde 
				Zombie* CreateHorde(int numZombies, sf::IntRect arena);	
				numZombies = 10;

				// delete the previously allocated memory!!!
				delete[] zombies;
				zombies = CreateHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				// reset the clock
				clock.restart();
			}
		}//END LEVELING UP! 
		
		/*
		UPDATE THE FRAME
		*/	
		if (state==EGameState::PLAYING)
		{
			//update delta time
			sf::Time deltaTime = clock.restart();
			gameTimeTotal += deltaTime;
			// make decimal fraction of 1 from the deltatime
			float deltaTimeAsSeconds = deltaTime.asSeconds();

			mouseScreenPosition = sf::Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(sf::Mouse::getPosition(), mainView);

			// set crosshair location updated
			sprCrosshair.setPosition(mouseWorldPosition);
			
			// update player
			player.Update(deltaTimeAsSeconds, sf::Mouse::getPosition());
			// player position
			sf::Vector2f playerPosition(player.GetCenter());
			// make view around player
			mainView.setCenter(player.GetCenter());

			// loop through each Zombie and update them
			for(int i=0;i<numZombies;i++)
			{
				if(zombies[i].IsAlive())
				{
					zombies[i].update(deltaTime.asSeconds(), playerPosition);
				}
			}
			// updating the bullet!
			for (int i=0;i<100;i++)
			{
				if(bullets[i].IsInFly())
				{
					bullets[i].Update(deltaTime.asSeconds());
				}
			}
			// update pickups 
			healthPickup.Update(deltaTimeAsSeconds);
			AmmoPickup.Update(deltaTimeAsSeconds);

			// Collision detection bullets hits Zombies
			// para todas las balas
			for(int i= 0;i<100;i++) // TODO 100 numero de balas
			{
				// para todos los zombies
				for (int j= 0; j<numZombies;j++)
				{
					if(bullets[i].IsInFly()&& zombies[j].IsAlive())
					{
						if(bullets[i].GetPosition().intersects(zombies[j].GetPosition()))
						{
							bullets[i].StopBullet();
							if(zombies[j].Hit())
							{
								Score += 10; // TODO SET SCORE PER ZOMBIE
								if(Score > HiScore)
								{
									HiScore = Score;
								}
								numZombiesAlive--;
								if(numZombiesAlive==0)
								{
									state = EGameState::LEVELING_UP;
								}
							}
						}
					}
				}
			}// END ZOMBIES BEING HIT

			//PLayer Beign Hit by Zombies
			for(int i=0;i<numZombies;i++)
			{
				if(player.GetPosition().intersects(zombies[i].GetPosition())&& zombies[i].IsAlive())
				{
					if(player.Hit(gameTimeTotal))
					{
						//TODO FAlta codigo
					}
					if(player.GetHealth()<=0)
					{
						state = EGameState::GAME_OVER;
						std::ofstream OutputFile("gamedata/hiscore.txt");
						OutputFile << HiScore;
						OutputFile.close();
					}
				}
			}// end Player hit by zombie

			// PICKUP BEIGN GET!!!
			if(player.GetPosition().intersects(healthPickup.GetPosition())&& healthPickup.IsSpawned())
			{
				player.GetMaxHealth(healthPickup.GotIt());
			}
			if(player.GetPosition().intersects(AmmoPickup.GetPosition())&& AmmoPickup.IsSpawned())
			{
				BulletSpare += AmmoPickup.GotIt();
			}

			
			// Size up the health bar
			healthBar.setSize(sf::Vector2f(player.GetHealth() * 3, 50));
			// increment the number of frames since prev update.
			framesSinceLastHudUpdate++;

			if(framesSinceLastHudUpdate> fpsMeasureFrameInterval)
			{
			//update game HUD Text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				ssAmmo << BulletInClip << "/" << BulletSpare;
				AmmoTxt.setString(ssAmmo.str());

				ssScore << "Score :" << Score;
				ScoreTxt.setString(ssScore.str());

				ssHiScore << "Hiscore :" << HiScore;
				HiscoreTxt.setString(ssHiScore.str());

				ssWave << "WAVE :" << wave;
				WaveTxt.setString(ssWave.str());

				ssZombiesAlive << "Zombies :" << numZombiesAlive;
				ZombiesRemainingTxt.setString(ssZombiesAlive.str());

				framesSinceLastHudUpdate = 0;
						
			}// end of HUD update

		}// END UPDATING SCENE		

				
		/*
		DRAW THE SCENE
		*/
		if (state == EGameState::PLAYING) 
		{
			window.clear();
			window.setView(mainView);
			window.draw(background, &textureBackground);
			window.draw(player.GetSprite());
			if(AmmoPickup.IsSpawned())
			{
				window.draw(AmmoPickup.GetSprite());
			}
			if(healthPickup.IsSpawned())
			{
				window.draw(healthPickup.GetSprite());
			}		
			//horde of zombies!
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].GetSprite());
			}
			// draw bullets!
			for (int i=0;i<100;i++)
			{
				if(bullets[i].IsInFly())
				{
					window.draw(bullets[i].GetShape());
				}
			}
			window.draw(sprCrosshair);

			// Switch to the hud view
			window.setView(HudView);

			// Draw all hud elements
			window.draw(sprAmmoHud);
			window.draw(AmmoTxt);
			window.draw(ScoreTxt);
			window.draw(HiscoreTxt);
			window.draw(healthBar);
			window.draw(WaveTxt);
			window.draw(ZombiesRemainingTxt);
		}
		if (state == EGameState::LEVELING_UP)
		{
			window.clear();
			window.draw(sprGameOver);
			window.draw(LevelUpTxt);
		}
		if (state == EGameState::PAUSED)
		{
			window.draw(PauseTxt);
		}
		if (state == EGameState::GAME_OVER)
		{
			window.clear();
			window.draw(sprGameOver);
			window.draw(GameOverTxt);
			window.draw(HiscoreTxt);
		}
		window.display();
			
	} // END GAME LOOP! 
	return 0;
}
