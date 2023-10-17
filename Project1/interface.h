#pragma once
#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <string>
#include <SFML/Graphics.hpp>
#include "block_enum.h"
#include "res.h"

class InterfaceManager
{	
	TextureResources* m_textures;
	// Sprites & Text
	sf::Sprite m_selected_item;
	sf::Sprite m_toolbar;
	sf::Sprite m_items[9];
	sf::Sprite m_selection_square;
	sf::Text m_fps_text;
	// Div
	int m_current_selection;
	int m_alpha_cd;
	// Const
	const std::string TOOL_BAR_CONTENT[9] {
		"item_redstone",
		"item_repeater",
		"item_redstone_block",
		"item_iron_block",
		"item_redstone_torch",
		"",
		"",
		"",
		""
	};
	const BlockType TOOLBAR_BLOCK_TYPES[9] {
		BLOCK_REDSTONE_WIRE,
		BLOCK_REPEATER,
		BLOCK_REDSTONE_BLOCK,
		BLOCK_IRON_BLOCK,
		BLOCK_TORCH,
		BLOCK_AIR,
		BLOCK_AIR,
		BLOCK_AIR,
		BLOCK_AIR,
	};

public:

	InterfaceManager(TextureResources* textures);
	void update(int ups);
	BlockType change_selection(int toolbar_case);
	BlockType scroll(bool foward);
	void set_cursor(sf::Vector2f pos);
	void draw(sf::RenderWindow& window);
};

#endif // !INTERFACE_H_INCLUDED
