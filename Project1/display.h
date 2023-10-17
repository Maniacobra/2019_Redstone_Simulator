#pragma once
#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "block.h"
#include "interface.h"

class Chunk
{
	sf::Sprite m_sprite;
	sf::RenderTexture m_texture;
	sf::IntRect m_rect;
	std::vector<MapBlock*> m_blocks;
	int m_drawed_blocks;

public:

	Chunk(sf::Vector2i pos);
	// Methods
	bool add_block(MapBlock* block);
	void display(void);
	void draw(sf::RenderWindow& window);
	void move(sf::Vector2f direction);
	void draw_block(MapBlock* block);
	// Cont
	bool is_inWindow(sf::FloatRect draw_zone);
};

class Displayer
{	
	sf::RenderWindow m_window;
	InterfaceManager* m_game_interface;
	std::vector<Chunk*> m_all_chunks;
	std::map<sf::Vector2i, Chunk*, Utils::Vec2i_predicate> m_chunks_grid;
	sf::View m_view;
	int m_size;
	struct Events
	{
		bool mouse_left;
		bool mouse_right;
		bool mouse_middle;
		std::vector<int> keys;
		bool scroll_up;
		bool scroll_down;
	} m_events;

public:

	Displayer(int win_x=WIN_X, int win_y=WIN_Y);
	// Methods
	void set_interface(InterfaceManager* game_interface);
	bool update(void);
	void zoom(int ticks);
	void resize(void);
	void add_chunk(sf::Vector2i pos);
	void set_chunks(int size);
	bool add_block(MapBlock* block);
	void set_blocks(int size, std::vector<MapBlock*> & blocks);
	void draw_block(MapBlock* block);
	void reset_events(void);
	sf::Vector2f get_mouse_pos(void);
	// Const
	bool running(void) const;
	bool mouse_click(int button) const;
	bool key_pressed(int key) const;
	bool mouse_pressed(int button) const;
	bool mouse_scrolled(bool upward) const;
};

#endif // !DISPLAY_H_INCLUDED