#include "utils.h"
#include "display.h"

Chunk::Chunk(sf::Vector2i pos)
{	
	pos *= CHUNKS_SIZE;
	m_rect = sf::IntRect(pos, sf::Vector2i(CHUNKS_SIZE, CHUNKS_SIZE));
	m_texture.create(CHUNKS_SIZE * SPRITE_SIZE, CHUNKS_SIZE * SPRITE_SIZE);
	m_sprite.setPosition(sf::Vector2f(
		m_rect.left < 0 ? (float)m_rect.left + 0.5f : (float)m_rect.left - 0.5f,
		(float)m_rect.top < 0 ? (float)m_rect.top + 0.5f : m_rect.top - 0.5f)
		* (float)SPRITE_SIZE);
}

bool Chunk::add_block(MapBlock* block)
{	
	sf::Vector2i pos(block->get_pos());
	if (pos.x < 0 && pos.x % CHUNKS_SIZE == 0)
		pos.x -= CHUNKS_SIZE;
	if (pos.y < 0 && pos.y % CHUNKS_SIZE == 0)
		pos.y -= CHUNKS_SIZE;
	if (m_rect.contains(pos))
	{
		m_blocks.push_back(block);
		return true;
	}
	return false;
}

void Chunk::display(void)
{	
	for (auto it(m_blocks.begin()); it != m_blocks.end(); ++it)
		(*it)->draw(m_texture);
	m_texture.display();
	m_sprite.setTexture(m_texture.getTexture());
}

void Chunk::draw(sf::RenderWindow& window)
{	
	window.draw(m_sprite);
}

void Chunk::move(sf::Vector2f direction)
{
	m_sprite.move(direction);
}

void Chunk::draw_block(MapBlock* block)
{	
	block->draw(m_texture);
	m_texture.display();
	m_sprite.setTexture(m_texture.getTexture());
	m_drawed_blocks++;
}

bool Chunk::is_inWindow(sf::FloatRect draw_zone)
{
	if (draw_zone.intersects(m_sprite.getGlobalBounds()))
		return true;
	return false;
}
