#pragma once
#ifndef RES_H_INCLUDED
#define RES_H_INCLUDED

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

class TextureGroup
{
	std::vector<sf::Texture*> m_group;
	sf::Texture* m_default_texture;

public:

	TextureGroup(sf::Texture* default_texture, std::vector<sf::Texture>& group)
	{
		for (auto it(group.begin()); it != group.end(); ++it)
			m_group.push_back(&*it);
		m_default_texture = default_texture;
	}
	TextureGroup(sf::Texture* default_texture)
	{
		m_default_texture = default_texture;
	}
	TextureGroup(void) {};
	sf::Texture* operator[](unsigned char nb)
	{
		if (nb < m_group.size())
			return m_group[nb];
		return m_default_texture;
	}
};

class TextureResources
{
	std::map<std::string, std::vector<sf::Texture> > m_textures;
	std::map<std::string, sf::Font> m_fonts;
	sf::Texture m_default_texture;

public:

	TextureResources(void);
	sf::Texture& get_texture(std::string file_name, unsigned int nb);
	TextureGroup get_group(std::string file_name);
	sf::Font& get_font(std::string file_name);
};

const std::string TEXTURE_FILES(
	"blocks/debug.1"
	"blocks/grass.1"
	"blocks/stone.1"
	"blocks/bedrock.1"
	"blocks/redstone.5"
	"blocks/repeater.16"
	"blocks/torch.3"
	"blocks/iron_block.1"
	"blocks/redstone_block.1"
	"blocks/lamp.2"
	"items/item_redstone.1"
	"items/item_repeater.1"
	"items/item_redstone_block.1"
	"items/item_iron_block.1"
	"items/item_redstone_torch.1"
	"ui/toolbar.1"
	"ui/select_square.1"
);

const std::string DEFAULT_FONT_FILE = "resources/fonts/consola.ttf";

#endif // !RES_H_INCLUDED
