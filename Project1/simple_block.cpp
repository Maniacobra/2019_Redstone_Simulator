#include "utils.h"
#include "behaviors.h"

void SimpleBlock::pre_init(void)
{
	m_alimented = false;
	m_charged = false;
}

void SimpleBlock::post_init(TextureResources& textures, std::string param)
{	
	m_textures = textures.get_group(param);
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
			if (m_connexions[i]->is_sensitive())
				m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL * m_alimented);
			else
				m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL * m_charged);
}

void SimpleBlock::on_destroy(void)
{
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
			m_connexions[i]->signal(this, Utils::swapside(i), 0);
}

void SimpleBlock::on_interaction_add(BlockOrientation side)
{	
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
			if (m_connexions[i]->is_sensitive())
				m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL * m_alimented);
			else
				m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL * m_charged);
}

void SimpleBlock::signal(BlockBehavior* block, BlockOrientation side, int level)
{	
	if (m_sources[side] == level || block->is_indirect())
		return;
	m_sources[side] = level;
	m_alimented = false;
	m_charged = false;
	for (int i = 0; i < 4; i++)
		if (m_sources[i] > 0)
		{	
			m_alimented = true;
			if (m_connexions[i] != nullptr && m_connexions[i]->get_id() == BLOCK_REPEATER)
				m_charged = true;
		}
	for (int i = 0; i < 4; i++)
		if (m_connexions[i] != nullptr)
		{	
			if (m_connexions[i]->is_sensitive())
				m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL * m_alimented);
			else
				m_connexions[i]->signal(this, Utils::swapside(i), REDSTONE_MAX_SIGNAL * m_charged);
		}
}

void SimpleBlock::draw(sf::Sprite& sprite)
{	
	sprite.setTexture(*m_textures[0]);
}