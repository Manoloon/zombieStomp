#include "stdafx.h"
#include "TextureHolder.h"

//assert feature
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

sf::Texture& TextureHolder::GetTexture(std::string const& filename)
{
// get reference to m_texture using m_s_instance
	auto& m = m_s_Instance->m_Textures;
	// auto  = string , texture
	
	// create an iterator to hold Key-value-pair (kvp)
	auto KeyValuePair = m.find(filename);
	//auto = string , texture
	
	// did we find a match?
	if (KeyValuePair != m.end())
	{
	// yes , return the texture 
		return KeyValuePair->second;
	}
	else 
	{
	//file not found
	// create a new Kvp using filename
		auto& texture = m[filename];
		texture.loadFromFile(filename);

		return texture;
	}
}
