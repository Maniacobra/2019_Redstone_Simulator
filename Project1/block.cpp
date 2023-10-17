#include <SFML/Graphics.hpp>
#include "utils.h"
#include "block.h"

MapBlock::MapBlock(sf::Texture* ground, sf::Vector2i grid_pos, sf::Color ground_color, bool bedrock)
{	
	m_ground = ground;
	m_pos = grid_pos;
	m_color = ground_color;
	m_pixel_pos = sf::Vector2f(abs(m_pos.x) % CHUNKS_SIZE * (float)SPRITE_SIZE, abs(m_pos.y) % CHUNKS_SIZE * (float)SPRITE_SIZE);
	if (m_pos.x < 0)
		m_pixel_pos.x = (float)(CHUNKS_SIZE - 1) * (float)SPRITE_SIZE - m_pixel_pos.x;
	if (m_pos.y < 0)
		m_pixel_pos.y = (float)(CHUNKS_SIZE - 1) * (float)SPRITE_SIZE - m_pixel_pos.y;
	m_highlighted = false;
	m_drawable = false;
	m_behavior = new BlockBehavior();
	m_bedrock = bedrock;
}

void MapBlock::set_chunk_pos(sf::Vector2i chunk_pos)
{
	m_chunk_pos = chunk_pos;
	m_drawable = true;
}

void MapBlock::highlight(void)
{	
	if (!m_bedrock)
		m_highlighted = true;
}

void MapBlock::unhighlight(void)
{
	m_highlighted = false;
}

void MapBlock::set_behavior(BlockBehavior* behavior)
{	
	delete m_behavior;
	m_behavior = behavior;
}

void MapBlock::draw(sf::RenderTexture& chunk_texture) const
{	
	if (!m_drawable)
		return;
	// Top
	sf::Sprite sprite_top;
	sprite_top.setPosition(m_pixel_pos);
	m_behavior->draw(sprite_top);
	// Ground
	sf::Sprite sprite_ground;
	sprite_ground.setPosition(m_pixel_pos);
	sprite_ground.setTexture(*m_ground);
	sprite_ground.setColor(m_color);
	// Draw
	chunk_texture.draw(sprite_ground);
	chunk_texture.draw(sprite_top);
	// Highlight
	if (m_highlighted)
	{
		sf::RectangleShape hl_overlay;
		hl_overlay.setFillColor(sf::Color(255, 255, 255, 100));
		hl_overlay.setSize(sf::Vector2f(SPRITE_SIZE, SPRITE_SIZE));
		hl_overlay.setPosition(m_pixel_pos);
		chunk_texture.draw(hl_overlay);
	}
}

bool MapBlock::is_bedrock(void) const
{
	return m_bedrock;
}

sf::Vector2i MapBlock::get_pos(void) const
{
	return m_pos;
}

sf::Vector2i MapBlock::get_chunk_pos(void) const
{
	return m_chunk_pos;
}

bool MapBlock::is_drawable(void) const
{
	return m_drawable;
}

BlockBehavior* MapBlock::get_behavior(void) const
{
	return m_behavior;
}
