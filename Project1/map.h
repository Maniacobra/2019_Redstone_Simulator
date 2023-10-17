#pragma once
#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <list>
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "block.h"
#include "display.h"
#include "res.h"

class GameMap
{	
	TextureResources* m_textures;
	std::vector<MapBlock*> m_all_blocks;
	std::list<MapBlock*> m_active_blocks;
	std::list<BlockBehavior*> m_ticked_behaviors;
	std::map<sf::Vector2i, MapBlock*, Utils::Vec2i_predicate> m_grid;
	int m_size;
	int m_nb;
	long m_last_second;
	int m_current_updates;
	int m_ups;
	int m_last_tick;

public:

	GameMap(TextureResources& m_textures, std::string ground, std::string border, int size, sf::Color ground_color);
	// Methods
	void update(Displayer& displayer);
	void assign_display(Displayer& displayer);
	MapBlock* get_block(sf::Vector2f pos);
	MapBlock* get_block(int x, int y);
	void place_block(MapBlock* block, int behavior_id, BlockOrientation rot=TOP, std::string param="", bool auto_rotate=true);
	void rot_block(MapBlock* block, bool clockwise);
	// Const
	int get_ups(void) const;
};

#endif // !MAP_H_INCLUDED
