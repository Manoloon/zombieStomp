// Create level background with Vertex Array.

#pragma once
#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "ZombieStomp.h"
// funcion crea el background .
int CreateBackground(sf::VertexArray& rVertexArray, sf::IntRect arena)
{
//todo lo que hacemos con rVertexArray , en realidad se lo estaremos haciendo a la background, en la funcion principal.
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int WorldWidth = arena.width / TILE_SIZE;
	int WorldHeight = arena.height / TILE_SIZE;

	// tipo de primitiva - cuadrado
	rVertexArray.setPrimitiveType(sf::Quads);

	rVertexArray.resize(WorldWidth * WorldHeight* VERTS_IN_QUAD);

	int currentVertex = 0;

	for(int w=0;w<WorldWidth;w++)
	{
		for (int h = 0; h < WorldHeight; h++) 
		{
			rVertexArray[currentVertex + 0].position = sf::Vector2f(w*TILE_SIZE, h*TILE_SIZE);
			rVertexArray[currentVertex + 1].position = sf::Vector2f((w*TILE_SIZE) + TILE_SIZE, h*TILE_SIZE);
			rVertexArray[currentVertex + 2].position = sf::Vector2f((w*TILE_SIZE) + TILE_SIZE, (h*TILE_SIZE) + TILE_SIZE);
			rVertexArray[currentVertex + 3].position = sf::Vector2f((w*TILE_SIZE), (h*TILE_SIZE) + TILE_SIZE);

			//define the position in the TEXTURE for current quad.
			// elije entre grass,stone,bush or wall
			if(h==0 ||h==WorldHeight-1 || w==0 || w==WorldWidth-1)
			{
			// use wall as texture if true.
				rVertexArray[currentVertex + 0].texCoords = sf::Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVertexArray[currentVertex + 1].texCoords = sf::Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVertexArray[currentVertex + 2].texCoords = sf::Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVertexArray[currentVertex + 3].texCoords = sf::Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}
			else 
			{
			// use random floor texture for Mug or Grass
				srand((int)time(0) + h * w - h);
				int MugOrGrass = (rand() % TILE_TYPES);
				int VerticalOffset = MugOrGrass * TILE_SIZE;

				rVertexArray[currentVertex + 0].texCoords = sf::Vector2f(0, 0 + VerticalOffset);
				rVertexArray[currentVertex + 1].texCoords = sf::Vector2f(TILE_SIZE, 0 + VerticalOffset);
				rVertexArray[currentVertex + 2].texCoords = sf::Vector2f(TILE_SIZE, TILE_SIZE + VerticalOffset);
				rVertexArray[currentVertex + 3].texCoords = sf::Vector2f(0, TILE_SIZE + VerticalOffset);
			}
			// position ready for the next vertice! 
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}
	return TILE_SIZE;

}