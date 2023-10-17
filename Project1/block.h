#pragma once
#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "behaviors.h"

class MapBlock
{	
	sf::Texture* m_ground;
	sf::Vector2i m_pos;
	sf::Vector2f m_pixel_pos;
	sf::Vector2i m_chunk_pos;
	BlockBehavior* m_behavior;
	bool m_highlighted;
	bool m_drawable;
	sf::Color m_color;
	bool m_bedrock;

public:

	MapBlock(sf::Texture* ground, sf::Vector2i grid_pos, sf::Color ground_color, bool bedrock);
	// Methods
	void set_chunk_pos(sf::Vector2i chunk_pos);
	void highlight(void);
	void unhighlight(void);
	void set_behavior(BlockBehavior* behavior);
	// Const
	void draw(sf::RenderTexture& chunk_texture) const;
	bool is_bedrock(void) const;
	sf::Vector2i get_pos(void) const;
	sf::Vector2i get_chunk_pos(void) const;
	bool is_drawable(void) const;
	BlockBehavior* get_behavior(void) const;
};

#endif // !BLOCK_H_INCLUDED